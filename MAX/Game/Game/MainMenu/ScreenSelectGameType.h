//
//  ScreenSelectGameType.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenSelectGameType__
#define __MAX__ScreenSelectGameType__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"
#include "ScreenSelectGameTypeDelegate.h"

using namespace cocos2d;

class ScreenSelectGameType : public ScreenMenuElement
{   

public:

	ScreenSelectGameTypeDelegate *_delegete_w;
	
    ScreenSelectGameType();
    virtual ~ScreenSelectGameType();
    
#pragma mark - Button events
    
	void OnTutorial(CCObject* sender);
	void OnHotseat(CCObject* sender);
	void OnMultiplayer(CCObject* sender);
	void OnEditor(CCObject* sender);
	void OnInfo(CCObject* sender);
   
};

#endif /* defined(__MAX__ScreenSelectGameType__) */
