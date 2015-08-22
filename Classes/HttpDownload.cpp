//
//  HttpDownload.cpp
//  mhzx
//
//  Created by ArcherPeng on 15/8/22.
//
//

#include "HttpDownload.h"
#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include "../cocos2d/external/unzip/unzip.h"
#endif
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

HttpDownload * HttpDownload::_instance = nullptr;
HttpDownload * HttpDownload::getInstance()
{
    if (!HttpDownload::_instance)
    {
        HttpDownload::_instance = HttpDownload::create();
        HttpDownload::_instance->retain();
    }
    return HttpDownload::_instance;
}

void HttpDownload::destroyInstance()
{
    HttpDownload::_instance->release();
    HttpDownload::_instance = nullptr;
}
HttpDownload::HttpDownload():_isStop(true),_delegate(nullptr),_url(""),_curlDown(nullptr),_stopDownload(false),_isAutoUncompress(false)
{
    _filePath = FileUtils::getInstance()->getWritablePath();
}
HttpDownload::~HttpDownload()
{
    if (this->_curlDown)
    {
        delete this->_curlDown;
        this->_curlDown = nullptr;
    }
}
#pragma mark- 下载出错回调
void HttpDownload::onError(CurlDown::ErrorCode errorCode){
    
    _isStop = true;
    //进入Cocos主线程然后分发
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([this,errorCode]
                                            {
                                                //TODO:分发给LUA
                                                
                                                CURLDOWNLOG("Download Error  ErrorCode:%d",(int)errorCode);//CurlDown::ErrorCode::kUncompress是停止
                                                if (_delegate)
                                                {
                                                    _delegate->onError(errorCode);
                                                    
                                                }
                                            });
    
}
#pragma mark- 下载进度回调
void HttpDownload::onProgress(double percent, void *delegate, string filefullPath){ // 下载进度
    
    if (_stopDownload) {
        CurlDown * cd = (CurlDown *)delegate;
        cd->setStopDown();
        _stopDownload = false;
    }
    //进入Cocos主线程然后分发
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([this,percent,delegate,filefullPath]
                                            {
                                                //TODO:分发给LUA
                                                
                                                CURLDOWNLOG("Download Progress %.2f%%",percent);
                                                if (_delegate)
                                                {
                                                    _delegate->onProgress(percent,delegate,filefullPath);
                                                    
                                                }
                                            });
}

#pragma mark- 下载成功回调
void HttpDownload::onSuccess(string filefullPath){
    
    _isStop = true;
    if(_isAutoUncompress)
    {
        if(uncompress())
        {
            this->deleteFile(_filePath + _url.substr(_url.rfind('/') + 1));
            //进入Cocos主线程然后分发
            auto scheduler = Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread([this,filefullPath]
                                                    {
                                                        //TODO:分发给LUA
                                                        
                                                        CURLDOWNLOG("%s Download Success",filefullPath.c_str());
                                                        if (_delegate)
                                                        {
                                                            _delegate->onSuccess(filefullPath);
                                                            
                                                        }
                                                    });

        }
        else
        {
            this->deleteFile(_filePath + _url.substr(_url.rfind('/') + 1));
            this->onError(CurlDown::ErrorCode::kUncompress);//解压失败
        }
    }
    else
    {
        //进入Cocos主线程然后分发
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([this,filefullPath]
                                                {
                                                    //TODO:分发给LUA
                                                    
                                                    CURLDOWNLOG("%s Download Success",filefullPath.c_str());
                                                    if (_delegate)
                                                    {
                                                        _delegate->onSuccess(filefullPath);
                                                        
                                                    }
                                                });
    }
    
}
#pragma mark- 停止下载的回调
void HttpDownload::onStop(string filefullPath)
{
    _isStop = true;
    //进入Cocos主线程然后分发
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([this,filefullPath]
                                            {
                                                //TODO:分发给LUA
                                                
                                                CURLDOWNLOG("%s Download Stop",filefullPath.c_str());
                                                if (_delegate)
                                                {
                                                    _delegate->onStop(filefullPath);
                                                    
                                                }
                                            });
}
#pragma mark- 开始下载
bool HttpDownload::download(std::string url,std::string filePath)
{
    if(_isStop)
    {
        
        this->_url = url;
        if (filePath != "")
        {
            this->_filePath=filePath;
        }
        else
        {
            this->_filePath=FileUtils::getInstance()->getWritablePath();
        }
        auto t = std::thread(&HttpDownload::downloadStartThread, this);
        t.detach();
        _isStop = false;
        CURLDOWNLOG("开始下载");
        return true;
    }
    else
    {
        CURLDOWNLOG("下载已开始");
        return false;
    }
}
#pragma mark- 下载文件，并自动解压缩
bool HttpDownload::downloadAndUncompress(std::string url,std::string filePath)
{
    if(this->download(url,filePath))
    {
        _isAutoUncompress = true;
        return true;
    }
    else
    {
        return false;
    }
}
#pragma mark- 解压缩的方法
bool HttpDownload::uncompress()
{
    // Open the zip file
    string outFileName = _filePath + _url.substr(_url.rfind('/') + 1);
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        const string fullPath = _filePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            //There are not directory entry in some case.
            //So we need to test whether the file directory exists when uncompressing file entry
            //, if does not exist then create directory
            const string fileNameStr(fileName);
            
            size_t startIndex=0;
            
            size_t index=fileNameStr.find("/",startIndex);
            
            while(index != std::string::npos)
            {
                const string dir=_filePath+fileNameStr.substr(0,index);
                
                FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(dir).c_str(), "r");
                
                if(!out)
                {
                    if (!createDirectory(dir.c_str()))
                    {
                        CCLOG("can not create directory %s", dir.c_str());
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }
                }
                else
                {
                    fclose(out);
                }
                
                startIndex=index+1;
                
                index=fileNameStr.find("/",startIndex);
                
            }
            
            
            
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    unzClose(zipfile);
    
    return true;

}
#pragma mark- 创建文件目录
bool HttpDownload::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    return FileUtils::getInstance()->createDirectory(_storagePath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#else
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#endif
    
    
}
#pragma mark- 开始下载的线程，通过这个线程启动下载
void HttpDownload::stopDownload()
{
    if (!_isStop)
    {
        _stopDownload = true;
    }
    
}
long HttpDownload::getDownloadFileSize(std::string url)const
{
    CurlDown *curl = new CurlDown();
    curl->mDownloadUrl = url;
    return curl->getDownloadFileLenth();
}
long HttpDownload::getLocalFileLength(std::string filePath)const
{
    CurlDown *curl = new CurlDown();
    curl->_FilePath = filePath;
    return curl->getLocalFileLength();
}
#pragma mark- 开始下载的线程，通过这个线程启动下载
void HttpDownload::downloadStartThread()
{
    if (this->_curlDown)
    {
        delete this->_curlDown;
        this->_curlDown = nullptr;
    }
    this->_curlDown = new CurlDown(_url,_filePath);
    this->_curlDown->setDelegate(this);
    this->_curlDown->downloadControler();
}
#pragma mark- 开始下载的线程，通过这个线程启动下载
bool HttpDownload::deleteFile(std::string filePath)const
{
    if (_isStop) {
        if (access(filePath.c_str(), 0) == 0) {
            remove(filePath.c_str());
            //删除成功
            return true;
        }else{
            //这个文件不存在，视作删除成功
            return true;
        }
    }else{
        //整在下载中无法删除
        return false;
    }
}