//
//  HttpDownload.h
//  mhzx
//
//  Created by ArcherPeng on 15/8/22.
//
//

#ifndef __mhzx__HttpDownload__
#define __mhzx__HttpDownload__

#include <stdio.h>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif
#include "cocos2d.h"
#include "CurlDown.h"
USING_NS_CC;
class HttpDownload:public Ref,CurlDownDelegate
{
public:
    HttpDownload();
    ~HttpDownload();
    CREATE_FUNC(HttpDownload)
    static HttpDownload * getInstance();
    static void destroyInstance();
    void setDelegate(CurlDownDelegate * delegate){_delegate = delegate;};//设置一个代理，用于接受下载状态的回调，代理类必须继承CurlDownDelegate并实现其中的虚方法
    CurlDownDelegate * getDelegate()const{return _delegate;}
    
    
    #pragma mark- 下载方法
    virtual bool download(std::string url,std::string filePath = "");//开始下载，
    virtual void stopDownload();//停止下载
    virtual bool isDownloading()const{return !_isStop;}//是否处于下载状态
    virtual bool deleteFile(std::string filePath)const;
    virtual long getDownloadFileSize(std::string url)const;//获取服务器文件大小
    virtual long getLocalFileLength(std::string filePath)const;//获取本地已经下载文件的大小
    virtual bool downloadAndUncompress(std::string url,std::string filePath = "");//下载并自动解压缩
    virtual bool isAutoUncompress()const {return _isAutoUncompress;}
    virtual void setAutoUncompress(bool isAutoUncompress ) {_isAutoUncompress = isAutoUncompress;}
    virtual bool uncompress();//解压缩的方法
    bool createDirectory(const char *path);//创建文件目录
    
    
    
protected:
    bool _isAutoUncompress;//是否自动解压缩的标识 注意，开启自动解压缩的时候，当下载完成后不会立即产生下载完成的回调，而是会自动解压缩，解压成功后才会抛出下载完成的回调
    CurlDown *_curlDown;
    bool _isStop;//下载状态
    static HttpDownload * _instance;
    CurlDownDelegate *_delegate;
    std::string _url;//下载文件的URL
    std::string _filePath;//下载文件存储路径
    bool _stopDownload;//是否停止下载的标识(停止下载不能立刻结束进程，需要等本阶段下载结束后才能停止)
    
    virtual bool init(){return true;};
    
#pragma mark- 下载状态回调
    virtual void onError(CurlDown::ErrorCode errorCode);
    virtual void onProgress(double percent, void *delegate, string filefullPath);
    virtual void onSuccess(string filefullPath);
    virtual void onStop(string filefullPath);
    
#pragma mark- 启动下载的线程
    void downloadStartThread();//开始下载的线程，通过这个线程启动下载
};
#endif /* defined(__mhzx__HttpDownload__) */
