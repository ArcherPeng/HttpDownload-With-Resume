//
//  XORCrypto.cpp
//  mhzx
//
//  Created by ArcherPeng on 15/8/21.
//
//

#include "XORCrypto.h"
#include "XORCrypto.h"
#include "ccMacros.h"

USING_NS_CC;


XORCrypto::XORCrypto(void)
{
}

XORCrypto::~XORCrypto(void)
{
}

XORCrypto * XORCrypto::instance()
{
    static XORCrypto instance;
    return &instance;
}

char XORCrypto::encodeChar( char c, int key )
{
    return c^key;
}

char XORCrypto::decodeChar( char c, int key )
{
    return c^key;
}

void XORCrypto::encode( char *pstr,int *pkey, int keyLength )
{
    int len=strlen(pstr);
    for(int i=0;i<len;i++)
    {
        *(pstr+i)=encodeChar(*(pstr+i),pkey[i%keyLength]);
    }
}

void XORCrypto::decode( char *pstr,int *pkey, int keyLength )
{
    int len=strlen(pstr);
    for(int i=0;i<len;i++)
    {
        *(pstr+i)=decodeChar(*(pstr+i),pkey[i%keyLength]);
    }
}

unsigned char* XORCrypto::encode( const char* pszFileName, unsigned long * pSize, int *pkey, int keyLength )
{
    if(pszFileName==NULL || pSize==NULL)
    {
        CCLOG("Invalid parameters.");
        return NULL;
    }
    unsigned char * pBuffer = NULL;
    unsigned char * temp = new unsigned char[1];
    const char* pszMode = "r";
    size_t size = sizeof(unsigned char);
    
    *pSize = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pszFileName);
        FILE *fp = fopen(fullPath.c_str(), pszMode);
        CC_BREAK_IF(!fp);
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        for(int i=0; i<*pSize; i++)
        {
            fseek(fp,i,SEEK_SET);
            fread(temp,size, 1,fp);
            *temp = *temp^pkey[i%keyLength];
            *(pBuffer+i) = *temp;
        }
        //*pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
        
        CCLOG("%s", msg.c_str());
    }
    return pBuffer;
}

unsigned char* XORCrypto::decode( const char* pszFileName, unsigned long * pSize, int *pkey, int keyLength )
{
    return encode(pszFileName, pSize, pkey, keyLength);
}