//
//  CurlDown.h
//  mhzx
//
//  Created by ArcherPeng on 15/8/22.
//
//

#ifndef __muli_download__CurlDown__
#define __muli_download__CurlDown__

#include <iostream>
#include <unistd.h>
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../cocos2dx/platform/third_party/win32/curl/curl.h"
#else
#include "curl.h"
#endif
#include <stdio.h>
using namespace cocos2d;
using namespace std;
USING_NS_CC;


//#define CURLDOWNLOG(...)       do {} while (0)

#define CURLDOWNLOG(format, ...)      cocos2d::log(format, ##__VA_ARGS__)


class CurlDownDelegate;

class CurlDown : public Ref {
private:
    string mFileName; // 下载文件名称
    bool isStop;
public:
    ~CurlDown();
    CurlDown();
    CurlDown(string downUrl,string filePath);
    
    string _FilePath; // 本地存储地址
    double _FileLenth; // 下载文件大小
    string mDownloadUrl; // 下载URL
    long _timeout; // 请求超时时间 为了测试用 设置超时时间为2秒 如果是发正式版本 改为20秒超时时间
    bool download(); // 下载方法
    long getDownloadFileLenth(); // 获取下载文件大小方法
    void downloadControler(); // 下载控制方法
    long getLocalFileLength(); // 当前下载文件大小
    
    void setStopDown();// 停止下载
    void setDelegate(CurlDownDelegate * delegate);
    CurlDownDelegate *_Delegate;
    
    
    enum ErrorCode
    {
        // Error caused by creating a file to store downloaded data
        kCreateFile,
        /** Error caused by network
         -- network unavaivable
         -- timeout
         -- ...
         */
        kNetwork,
        /** There is not a new version
         */
        kNoNewVersion,
        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */
        kUncompress,
    };
};

class CurlDownDelegate
{
public:
    //出现错误的回调
    virtual void onError(CurlDown::ErrorCode errorCode) = 0;
    //下载进度的回调
    virtual void onProgress(double percent, void *delegate, string filefullPath) = 0;
    //下载成功回调
    virtual void onSuccess(string filefullPath) = 0;
    //用户停止下载回调
    virtual void onStop(string filefullPath) = 0;
};

#endif /* defined(__muli_download__CurlDown__) */
