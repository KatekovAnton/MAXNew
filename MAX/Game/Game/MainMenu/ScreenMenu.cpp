//
//  ScreenMenu.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "MAXContentLoader.h"
#include "ScreenMenu.h"
#include "ScreenSelectGameType.h"
#include "MAXMainMenuController.h"

#define BUTTON_LABEL_TAG 11

ScreenMenu::ScreenMenu()
	:_menuController(NULL)
{
}

ScreenMenu::~ScreenMenu()
{
}

void ScreenMenu::InitBaseInterface()
{
//ScreenSelectGameType
//start:
//0.Tutorial
//-Scanario list (tutorials) -> Game
//
//
//1.Hot seat
//a.Hew game -> Scenario list (scenaries) -> Game settings -> Game 
//b.Load game -> Scenario list (saves) -> Game
//
//
//2.Multiplayer
//-Not supported
//
//
//3.Editor
//a.New scenario -> Scenario settings -> Editor
//b.Edit scanario -> Scenario list (scenaries) -> Editor
//
//
//4.Info
//a.Credits
//b.Legal information and copyrights
//c.Privacy policy (for multiplayer)
//
//
	{
		CCSprite* turnSprite = MAXSCL->CreateSpriteFromPalettedImage("ENDGAME7");
		turnSprite->setAnchorPoint(ccp(0, 0));
		float y = getContentSize().height/2 - turnSprite->getContentSize().height/2;
		if (y < 0)
			y = 0;
	
		turnSprite->setPosition(ccp(getContentSize().width - 800, y));
		addChild(turnSprite);
	}
	{
		_startScreen = new ScreenSelectGameType();
		float y = getContentSize().height/2 - _startScreen->getContentSize().height/2;
		if (y < 0)
			y = 0;
	
		_startScreen->setPosition(ccp(getContentSize().width - 200, y));
		_startScreen->_delegete_w = _menuController;
		addChild(_startScreen);
	}
    
}



