//
//  HttpDownload.cpp
//  mhzx
//
//  Created by ArcherPeng on 15/8/22.
//
//

#include "HttpDownload.h"
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
HttpDownload::HttpDownload():_isStop(true),_delegate(nullptr),_url(""),_curlDown(nullptr),_stopDownload(false)
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