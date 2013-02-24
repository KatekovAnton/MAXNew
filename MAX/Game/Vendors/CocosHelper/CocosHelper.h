//
//  CocosHelper.h
//  MAX
//
//  Created by  Developer on 24.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__CocosHelper__
#define __MAX__CocosHelper__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class CocosHelper {
    
public:
    
    static void ProceedCCNode(CCNode* node);
    static void ProceedCCNodeBack(CCNode* node);

    static void SetPosition(CCNode* node, const CCPoint& point);
    static void SetContentSize(CCNode* node, const CCSize& size);
    
    
    static CCSize GetContentSize(CCNode* node);
};

#endif /* defined(__MAX__CocosHelper__) */
