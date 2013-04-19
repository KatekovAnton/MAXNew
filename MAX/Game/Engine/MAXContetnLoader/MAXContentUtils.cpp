//
//  MAXContentUtils.cpp
//  MAX
//
//  Created by Anton Katekov on 19.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXContentUtils.h"
#include "FileManger.h"
#include "cocos2d.h"

using namespace cocos2d;


MAXRESTextureData read_png_file(string fileName)
{
    FileManager* fm = FileManager::CreateManager();
    string path = fm->GetFilePath(fileName);
    delete fm;
    CCImage::EImageFormat eImageFormat = CCImage::kFmtPng;
       
    CCImage image;
    unsigned long nSize = 0;
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &nSize);
    if (! image.initWithImageData((void*)pBuffer, nSize, eImageFormat))
    {
        CC_SAFE_DELETE_ARRAY(pBuffer);
    }
    else
    {
        CC_SAFE_DELETE_ARRAY(pBuffer);
    }
    MAXRESTextureData result;
    result.data = (Color*)malloc(image.getDataLen() * 4);
    memcpy(result.data, image.getData(), image.getDataLen() * 4);
    result.h = image.getHeight();
    result.w = image.getWidth();
    return result;
}