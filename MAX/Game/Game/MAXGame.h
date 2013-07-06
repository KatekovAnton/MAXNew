//
//  MAXGame.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXGame__
#define __MAX__MAXGame__

#include <iostream>
#include "DisplayPinchDelegate.h"
#include "MAXGameController.h"
#include "USimpleContainer.h"

using namespace Utils; 

enum MAXGAMESTATE
{
    MAXGAMESTATE_LOADINGSTART,
    MAXGAMESTATE_MAINMENU,
    MAXGAMESTATE_EDITOR,
    MAXGAMESTATE_LOADINGGAME,
    MAXGAMESTATE_GAME
};

class MAXMainMenuController;

class MAXGame : public DisplayPinchDelegate
{
	MAXGAMESTATE _currentState;
    
public:
    
    MAXGameController *_gameController;
    MAXMainMenuController *_mainMenuController;

    MAXGAMESTATE GetCurrentState() const {return _currentState;}
    
    MAXGame();
    ~MAXGame();
    
    void Init();
	
    USimpleContainer<GameEffect*> *_effects;
	void FlushEffectsWithNew(GameEffect *effect);
    
    
#pragma mark - DisplayPinchDelegate
    virtual bool CanStartPinch(float x, float y);
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
    virtual void ProceedTap(float tapx, float tapy);
    virtual void ProceedLongTap(float tapx, float tapy);
    
};

extern MAXGame *  game;

#endif /* defined(__MAX__MAXGame__) */
