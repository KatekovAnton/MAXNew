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
#include "USimpleContainer.h"
#include "MAXEngine.h"
#include "SelectedGameObjectDelegate.h"

using namespace std;
using namespace Utils;

enum MAXGAMESTATE
{
    MAXGAMESTATE_LOADINGSTART,
    MAXGAMESTATE_MAINMENU,
    MAXGAMESTATE_LOADINGGAME,
    MAXGAMESTATE_GAME
};

class MAXEngine;
class GameMap;
class GameUnit;

class GameMatch;
class GameEffect;
class GameInterface;
class GamePathVisualizer;
class PFWaveCell;

class MAXGame : public DisplayPinchDelegate, public MAXEngineDelegate, public SelectedGameObjectDelegate
{
    
    MAXGAMESTATE _currentState;
    
    
    GameInterface *_gameInterface;
    GamePathVisualizer *_pathVisualizer;
    int iteration;
    void StartTest();
    void ShowPathMap();
    void HidePathMap();
	void ShowUnitPath(GameUnit *unit);
	void HideUnitPath();
    void RefreshCurrentUnitPath();
    bool CheckIfNextCellOk(GameUnit* unit);
    void RecalculateUnitPath(GameUnit* unit);
public:
    
    int _freezeCounter;
    
    GameMatch *_match;
    
    USimpleContainer<GameEffect*> *_effects;
    
    MAXGAMESTATE GetCurrentState() const {return _currentState;}
    GameMatch *GetCurrentMatch() const {return _match;}
    GameUnit *_currentUnit;
    
    GameUnit *_testUnit;
    
    MAXGame();
    ~MAXGame();
    
    void Init();
    
    
    void StartMatch();
    bool EndTurn();
    void FlushEffectsWithNew(GameEffect *effect);
  
#pragma mark - Interface
#pragma mark Messages
    void ShowUnitSpottedMessage(GameUnit* unit);
    
#pragma mark - MAXEngineDelegate
    virtual void onFrame();
    
#pragma mark - DisplayPinchDelegate
    virtual bool CanStartPinch(float x, float y);
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
    virtual void ProceedTap(float tapx, float tapy);
    virtual void ProceedLongTap(float tapx, float tapy);
    
#pragma mark - SelectedGameObjectDelegate
    virtual void onUnitMoveStart(GameUnit* unit);
	virtual void onUnitMovePause(GameUnit* unit);
	virtual void onUnitMoveStepBegin(GameUnit* unit);
	virtual void onUnitMoveStepEnd(GameUnit* unit);
    virtual void onUnitMoveStop(GameUnit* unit);
    virtual void onUnitFireStart(GameUnit* unit);
    virtual void onUnitFireStop(GameUnit* unit);
    
};

extern MAXGame *  game;

#endif /* defined(__MAX__MAXGame__) */
