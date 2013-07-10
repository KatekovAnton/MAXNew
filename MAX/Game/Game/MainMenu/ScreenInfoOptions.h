//
//  ScreenInfoOptions.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenInfoOptions__
#define __MAX__ScreenInfoOptions__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class ScreenInfoOptions : public ScreenMenuElement
{   

public:

    ScreenInfoOptions();
    virtual ~ScreenInfoOptions();
    
#pragma mark - Button events
	
	void OnCredits(CCObject *sender);
	void OnLegal(CCObject *sender);
	void OnPrivacy(CCObject *sender);
	void OnBack(CCObject *sender);
    
};

#endif /* defined(__MAX__ScreenInfoOptions__) */
