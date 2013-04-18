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
	void ShowUnitPath(std::vector<PFWaveCell*> path);
public:
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
    virtual void onUnitStartMove(GameUnit* unit);
    virtual void onUnitStopMove(GameUnit* unit);
    
};

extern MAXGame *  game;

#endif /* defined(__MAX__MAXGame__) */
