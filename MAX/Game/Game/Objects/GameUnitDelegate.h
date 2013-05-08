//
//  GameUnitDelegate.h
//  MAX
//
//  Created by  Developer on 03.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GameUnitDelegate_h
#define MAX_GameUnitDelegate_h

#include "cocos2d.h"

using namespace cocos2d;

class GameUnit;
class GameUnitParameters;

class GameUnitDelegate {
    
public:
    
    virtual void GameUnitWillLeaveCell(GameUnit *unit) = 0;
    virtual void GameUnitDidEnterCell(GameUnit *unit) = 0;
    
    virtual void GameUnitDidDestroy(GameUnit *unit) = 0;
    
    virtual void GameUnitDidPlaceOnMap(GameUnit *unit) = 0;
    virtual void GameUnitDidRemoveFromMap(GameUnit *unit) = 0;
    
    virtual void GameUnitDidUndetected(GameUnit *unit) = 0;
    virtual void GameUnitDidDetected(GameUnit *unit) = 0;
};

#endif
