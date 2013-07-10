//
//  MAXMainMenuController.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "MAXMainMenuController.h"
#include "MAXAnimationPrefix.h"
#include "ScreenMenu.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"
#include "ScreenSelectGameType.h"
#include "ScreenEditorOptions.h"
#include "ScreenScenarioList.h"
#include "ScreenInfoOptions.h"
#include "ScreenNotSupported.h"
#include "ScreenHotSeatOptions.h"
#include "ScreenProgressBar.h"
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

		ScreenSelectGameType *_startScreen = new ScreenSelectGameType();
		_startScreen->autorelease();
		_startScreen->_delegete_w = this;
		_sceneMenu->PushScreen(_startScreen);
	
	}
}

void MAXMainMenuController::End()
{
}

void MAXMainMenuController::ToStartState()
{
	_sceneMenu->PopToFirstScreen();
}

void MAXMainMenuController::StartMatch()
{	
	ScreenProgressBar *screen = new ScreenProgressBar();
	screen->autorelease();
	screen->_controller = this;
	_sceneMenu->PushScreen(screen);
}

void MAXMainMenuController::SetLoadingProgress(float zeroToOne)
{
	_loadingProgressBar->SetProgress(zeroToOne);
	
	engine->DrawStart();
	engine->DrawInterface();
	engine->EndFrame();
}

void MAXMainMenuController::LoadingScreenDidAppear(ScreenProgressBar *screen)
{
	_loadingProgressBar = screen;

	//this delay is required for isolate cocos update 
	//becouse this event calls from cocos sceduler, 
	//so we should finish this update of cocos engine
	MAXAnimationWait *wait = new MAXAnimationWait(0);
	wait->_delegate = this;
	MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
	_animWaitForStart = wait;
}

#pragma mark - ScreenSelectGameTypeDelegate

void MAXMainMenuController::OnTutorial()
{
	ScreenScenarioList* screen = new ScreenScenarioList();
	screen->autorelease();
	_sceneMenu->PushScreen(screen);
}

void MAXMainMenuController::OnHotseat()
{
	ScreenHotSeatOptions *screen = new ScreenHotSeatOptions();
	screen->autorelease();
	screen->_menuController = this;
	_sceneMenu->PushScreen(screen);
}

void MAXMainMenuController::OnMultiplayer()
{
	ScreenNotSupported* screen = new ScreenNotSupported();
	screen->autorelease();
	_sceneMenu->PushScreen(screen);
}

void MAXMainMenuController::OnEditor()
{
	ScreenEditorOptions* screen = new ScreenEditorOptions();
	screen->autorelease();
	_sceneMenu->PushScreen(screen);
}

void MAXMainMenuController::OnInfo()
{
	ScreenInfoOptions *screen = new ScreenInfoOptions();
	screen->autorelease();
	_sceneMenu->PushScreen(screen);
}

#pragma mark - MAXAnimationDelegate 
  
void MAXMainMenuController::OnAnimationStart(MAXAnimationBase* animation)
{}

void MAXMainMenuController::OnAnimationUpdate(MAXAnimationBase* animation)
{}

void MAXMainMenuController::OnAnimationFinish(MAXAnimationBase* animation)
{
	if (animation == _animWaitForStart)
	{
		_animWaitForStart = NULL;
		game->StartTestMatch();
	}
}
    
