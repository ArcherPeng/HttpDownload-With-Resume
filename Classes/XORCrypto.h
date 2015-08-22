//
//  XORCrypto.h
//  mhzx
//
//  Created by ArcherPeng on 15/8/21.
//
//

#ifndef __mhzx__XORCrypto__
#define __mhzx__XORCrypto__

#include <stdio.h>
#pragma once
#include <string>

#include "cocos2d.h"

using namespace std;

class XORCrypto
{
private:
    XORCrypto(void);
    ~XORCrypto(void);
public:
    static XORCrypto *instance();
    
    void encode(char *pstr,int *pkey, int keyLength);
    void decode(char *pstr,int *pkey, int keyLength);
    
    char encodeChar(char c, int key);
    char decodeChar(char c, int key);
    
    unsigned char* encode(const char* pszFileName, unsigned long * pSize, int *pkey, int keyLength);
    unsigned char* decode(const char* pszFileName, unsigned long * pSize, int *pkey, int keyLength);
    
};

#endif /* defined(__mhzx__XORCrypto__) */
