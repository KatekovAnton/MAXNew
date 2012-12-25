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

enum MAXGAMESTATE
{
    MAXGAMESTATE_LOADINGSTART,
    MAXGAMESTATE_MAINMENU,
    MAXGAMESTATE_LOADINGGAME,
    MAXGAMESTATE_GAME
};


class MAXEngine;

class MAXGame : public DisplayPinchDelegate {
    
    MAXGAMESTATE _curretnState;
public:
    
    MAXGAMESTATE GetCurretnState() const {return _curretnState;}
    
    MAXGame();
    ~MAXGame();
    
    void Init();
    
#pragma mark - DisplayPinchDelegate
    virtual bool CanStartPinch();
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
};

extern MAXGame *  game;

#endif /* defined(__MAX__MAXGame__) */
