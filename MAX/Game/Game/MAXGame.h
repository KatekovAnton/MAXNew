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

using namespace std;

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
class GameInterface;

class MAXGame : public DisplayPinchDelegate {
    
    MAXGAMESTATE _curretnState;
    
    
    GameInterface* _gameInterface;
    GameMatch* _match;
    
public:
    
    MAXGAMESTATE GetCurretnState() const {return _curretnState;}
    shared_ptr<GameUnit> _testUnit;
    
    MAXGame();
    ~MAXGame();
    
    void Init();
    
    
    void StartMatch();
    void SetMap(string mapName);
    
    
#pragma mark - DisplayPinchDelegate
    virtual bool CanStartPinch(float x, float y);
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
    virtual void ProceedTap(float tapx, float tapy);
    virtual void ProceedLongTap(float tapx, float tapy);
    
};

extern MAXGame *  game;

#endif /* defined(__MAX__MAXGame__) */
