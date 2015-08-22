#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CurlDown.h"

class HelloWorld : public cocos2d::Layer,CurlDownDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void httpThread(const char * str);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    void schCallback(float f);
    CurlDown *ccc;
    string downFilePath;
    bool isStop = true;
    // 断点续传 回调方法
    virtual void onError(CurlDown::ErrorCode errorCode);
    virtual void onProgress(double percent, void *delegate, string filefullPath);
    virtual void onSuccess(string filefullPath);
    virtual void onStop(string filefullPath);
    Label * label;
};

#endif // __HELLOWORLD_SCENE_H__
