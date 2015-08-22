#include "HelloWorldScene.h"
#include "XORCrypto.h"
#include "HttpDownload.h"
#include <unistd.h>
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    auto closeItem = MenuItemLabel::create(
                                           Label::createWithSystemFont("查看文件大小", "Arial", 18),
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2( origin.x+visibleSize.width/2 ,origin.y+visibleSize.height/8*3));
//    closeItem->setPosition(Vec2( 0 ,0));
    CCLOG("visibleSize.width/2   %f",visibleSize.width/2);
    
    closeItem->setTag(100);
    
    auto closeItem1 = MenuItemLabel::create(
                                            Label::createWithSystemFont("开始下载", "Arial", 18),
                                            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem1->setPosition(Vec2( origin.x+visibleSize.width/2 ,origin.y+visibleSize.height/8*5));
    
    
    closeItem1->setTag(101);
    
    auto closeItem2 = MenuItemLabel::create(
                                            Label::createWithSystemFont("停止下载", "Arial", 18),
                                            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem2->setPosition(Vec2( origin.x+visibleSize.width/2 ,origin.y+visibleSize.height/8*4));
    
    
    closeItem2->setTag(102);
    
    auto closeItem3 = MenuItemLabel::create(
                                            Label::createWithSystemFont("删除文件", "Arial", 18),
                                            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem3->setPosition(Vec2( origin.x+visibleSize.width/2 ,origin.y+visibleSize.height/8*2));
    
    
    closeItem3->setTag(103);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem,closeItem1,closeItem2,closeItem3, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 15);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    label->setAnchorPoint(Vec2(0.5, 1));
    label->setAlignment(TextHAlignment::CENTER);
    // add the label as a child to this layer
    this->addChild(label, 1);

//    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("001.png");
//
//    
//
//    
//    GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
//                                                                     FileUtils::getInstance()->fullPathForFilename("shader.fsh").c_str())->getCString();
////    auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_vert, fragSource);
////    CCLOG("%s",fragSource);
//    auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);
//    
////    uniform float x;
////    uniform float y;
////    uniform float width;
////    uniform float height;
////    uniform float frame;
//    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
//    sprite->setGLProgramState(glProgramState);
//    
//    sprite->getGLProgramState()->setUniformFloat("x", 0.5f);
//    sprite->getGLProgramState()->setUniformFloat("y", 0.0f);
//    sprite->getGLProgramState()->setUniformFloat("width", 750.0f);
//    sprite->getGLProgramState()->setUniformFloat("height", 1334.0f);
//    sprite->getGLProgramState()->setUniformFloat("frame", 100.0f);
//    
//    sprite->setLocalZOrder(100);
//    // add the sprite as a child to this layer
//    
//    // position the sprite on the center of the screen
//    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    this->addChild(sprite);
    
    
    auto sprite = Sprite::create("002.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    sprite->setScale(2);
    sprite->setName("sprite");
    this->addChild(sprite, 0);
    
//    schedule([&](float f){static int op = 255;sprite->setOpacity(op);op+=10;}, 10,"123");
    schedule(schedule_selector(HelloWorld::schCallback));
    
//    const char *str = "getFileDataFromZip";
//    int key[18];
//    for (int i = 0; i<18; ++i)
//    {
//        key[i]=str[i];
//    }
//    auto jsonpath = cocos2d::CCFileUtils::getInstance()->fullPathForFilename("contactqq.png");
//    unsigned long size = 0;
//    unsigned char* strD = XORCrypto::instance()->decode(jsonpath.c_str(), &size, key, 18);
//    string path = FileUtils::getInstance()->getWritablePath() + "dog1.png";
//    CCLOG("wanna save file path = %s  size = %lu",path.c_str(),size);
//    
//    
//    
//    //创建一个文件指针
//    //路径、模式
//    FILE* file = fopen(path.c_str(), "w");
//    if (file) {
////        fputs((char*)strD, file);
//        for (int i = 0 ; i<size; ++i)
//        {
//            fprintf(file, "%c",strD[i]);
//        }
//        fclose(file);
//    }

    
    
    this->downFilePath = FileUtils::getInstance()->getWritablePath();
    
    
    
    
    return true;
}
void HelloWorld::schCallback(float f)
{
    static int zheng = 5;
    static int op = 0;
    auto sp = this->getChildByName<Sprite*>("sprite");
    sp->setOpacity(op);
    op+=zheng;
    if (op>=255 || op<=0)
    {
        zheng= -zheng;
//        op = 0;
    }
}

void HelloWorld::httpThread(const char * str)
{
//    HelloWorld *hw = (HelloWorld*)arg;
    this->ccc = new CurlDown(str,this->downFilePath);
    //    ccc->mDownloadUrl = "http://developer.baidu.com/map/static/doc/output/BaiduMap_AndroidSDK_v2.4.0_All.zip";
    //    int leng = ccc->getDownloadFileLenth();
    this->ccc->setDelegate(this);
    this->ccc->downloadControler();
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    auto node = dynamic_cast<Node*>(pSender);
    switch (node->getTag())
    {
        case 100:
        {
            float dnSize =HttpDownload::getInstance()->getDownloadFileSize("http://aligame.oss-cn-hangzhou.aliyuncs.com/dkzjh.apk")/1024/1024;
            float localSize =HttpDownload::getInstance()->getLocalFileLength(downFilePath+"dkzjh.apk")/1024/1024;
            label->setString(StringUtils::format("Download File Size \n%.2fM \nLocal File Size \n%.2fM",dnSize,localSize));
        }
            break;
        case 101:
        {
//            HttpDownload::getInstance()->download("http://aligame.oss-cn-hangzhou.aliyuncs.com/dkzjh.apk");
//            http://developer.baidu.com/map/static/doc/output/BaiduMap_AndroidSDK_v2.4.0_All.zip
//            HttpDownload::getInstance()->downloadAndUncompress("http://developer.baidu.com/map/static/doc/output/BaiduMap_AndroidSDK_v2.4.0_All.zip");
            HttpDownload::getInstance()->downloadAndUncompress("http://aligame.oss-cn-hangzhou.aliyuncs.com/dkzjh.apk");
            HttpDownload::getInstance()->setDelegate(this);
            isStop = false;
            break;
            
        }
            
        case 102:
            HttpDownload::getInstance()->stopDownload();
            break;
            
        case 103:
            if (HttpDownload::getInstance()->deleteFile(downFilePath+"dkzjh.apk"))
            {
                MessageBox("删除成功", "温馨提示");
                label->setString("Delete Success!");
            }
            else
            {
                MessageBox("文件下载中，请先停止下载", "温馨提示");
            }
            break;
            
        default:
            break;
    }
    


}

void HelloWorld::onError(CurlDown::ErrorCode errorCode){
    label->setString(StringUtils::format("Download Error  %d",(int)errorCode));
    isStop = true;
}
void HelloWorld::onProgress(double percent, void *delegate, string filefullPath){ // 下载进度

    label->setString(StringUtils::format( "Downloading\n%.2f%%",percent));
}
void HelloWorld::onSuccess(string filefullPath){
    label->setString("Success");
    isStop = true;
}
void HelloWorld:: onStop(string filefullPath)
{
//    MessageBox("用户停止下载!", "温馨提示");
    label->setString(StringUtils::format( "Stop -- %s",label->getString().c_str()));
}
