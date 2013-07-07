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

using namespace cocos2d;

class ScreenSelectGameType : public CCNode
{   

public:

    ScreenSelectGameType();
    virtual ~ScreenSelectGameType();
    
#pragma mark - Button events
    
	void OnTutorial(CCMenuItem* sender);
	void OnHotseat(CCMenuItem* sender);
	void OnMultiplayer(CCMenuItem* sender);
	void OnEditor(CCMenuItem* sender);
	void OnInfo(CCMenuItem* sender);
   
};

#endif /* defined(__MAX__ScreenSelectGameType__) */
