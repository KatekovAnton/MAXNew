//
//  ScreenHotSeatOptions.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenHotSeatOptions__
#define __MAX__ScreenHotSeatOptions__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class MAXMainMenuController;

class ScreenHotSeatOptions : public ScreenMenuElement
{   

public:

	MAXMainMenuController *_menuController;

    ScreenHotSeatOptions();
    virtual ~ScreenHotSeatOptions();
    
#pragma mark - Button events
    
	void OnTestMatch(CCObject *sender);
	void OnBack(CCObject *sender);
   
};

#endif /* defined(__MAX__ScreenHotSeatOptions__) */
