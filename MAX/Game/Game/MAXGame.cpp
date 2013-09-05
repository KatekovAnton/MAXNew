//
//  MAXGame.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXGame.h"
#include "Display.h"
#include "cocos2d.h"


#include "MAXGameController.h"
#include "MAXMainMenuController.h"
#include "GameEffect.h"

MAXGame globalGame;
MAXGame * game = &globalGame;

MAXGame::MAXGame()
:_gameController(NULL), _mainMenuController(NULL)
{
    _effects = new USimpleContainer<GameEffect*>();
}

MAXGame::~MAXGame()
{
	if (_gameController)
		delete _gameController;
	
	for (int i = 0; i < _effects->GetCount(); i++) 
	{
        GameEffect *effect = _effects->objectAtIndex(i);
        delete effect;
    }
    delete _effects;
}

void MAXGame::Init()
{
    DEBUG_FUNCTION_ENTER(this);
    Display::currentDisplay()->SetPinchDelegate(this);

	_mainMenuController = new MAXMainMenuController();
	_mainMenuController->Begin();
	//_gameController = new MAXGameController();
	//_gameController->Init();
	_currentState = MAXGAMESTATE_MAINMENU;
    DEBUG_FUNCTION_EXIT;
}

void MAXGame::FlushEffectsWithNew(GameEffect *effect)
{
    bool onemorestep = true;
    while (onemorestep)
    {
        onemorestep = false;
        int count = _effects->GetCount();
        for (int i = 0; i < count; i++)
        {
            GameEffect* effect = _effects->objectAtIndex(i);
            if (effect->GetFinished())
            {
                _effects->remove(i);
                onemorestep = true;
                delete effect;
                break;
            }
        }
    }
}

void MAXGame::SetLoadingProgress(float zeroToOne)
{
	_mainMenuController->SetLoadingProgress(zeroToOne);
}

void MAXGame::StartTestMatch()
{
	if (_currentState == MAXGAMESTATE_MAINMENU)
	{
		
	}
	_gameController = new MAXGameController();
	_gameController->Init();
	_currentState = MAXGAMESTATE_GAME;
}

void MAXGame::StopMatch()
{
	delete _gameController;
	_gameController = NULL;
	_currentState = MAXGAMESTATE_MAINMENU;
	_mainMenuController->ToStartState();
}

#pragma mark - DisplayPinchDelegate
bool MAXGame::CanStartPinch(float x, float y)
{
	if (_gameController)
		return _gameController->CanStartPinch(x, y);
	
	return false;
}
void MAXGame::ProceedPinch(float scale)
{	
	if (_gameController)
		_gameController->ProceedPinch(scale);
}

void MAXGame::ProceedPan(float speedx, float speedy)
{
	if (_gameController)
		_gameController->ProceedPan(speedx, speedy);
}

void MAXGame::ProceedTap(float tapx, float tapy)
{
	if (_gameController)
		_gameController->ProceedTap(tapx, tapy);
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
	if (_gameController)
		_gameController->ProceedLongTap(tapx, tapy);
}

void MAXGame::onDeviceMoved(double x, double y)
{
    if (_mainMenuController) {
        _mainMenuController->onDeviceMoved(x,y);
    }
}
