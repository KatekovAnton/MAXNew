//
//  MAXMainMenuController.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "MAXMainMenuController.h"
#include "ScreenMenu.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"

#include "SoundEngine.h"

using namespace extension;

MAXMainMenuController::MAXMainMenuController()
	:_sceneMenu(NULL)
{
}

MAXMainMenuController::~MAXMainMenuController()
{
	if (_sceneMenu)
		_sceneMenu->autorelease();
}

void MAXMainMenuController::Begin()
{   
	if (!_sceneMenu)
	{
		_sceneMenu = new ScreenMenu();
		_sceneMenu->_menuController = this;
		_sceneMenu->init();
		_sceneMenu->InitBaseInterface();
		CCDirector::sharedDirector()->pushScene(_sceneMenu);
	}
}

void MAXMainMenuController::End()
{
}

#pragma mark - ScreenSelectGameTypeDelegate

void MAXMainMenuController::OnTutorial()
{}

void MAXMainMenuController::OnHotseat()
{
	game->StartTestMatch();
}

void MAXMainMenuController::OnMultiplayer()
{}

void MAXMainMenuController::OnEditor()
{}

void MAXMainMenuController::OnInfo()
{}


