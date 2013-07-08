//
//  ScreenMenu.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenMenu__
#define __MAX__ScreenMenu__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class MAXMainMenuController;
class ScreenSelectGameType;

class ScreenMenu : public CCScene
{   

public:

	MAXMainMenuController *_menuController;
	
	ScreenSelectGameType *_startScreen;

    ScreenMenu();
    ~ScreenMenu();
    
    void InitBaseInterface();

};

#endif /* defined(__MAX__ScreenMenu__) */
