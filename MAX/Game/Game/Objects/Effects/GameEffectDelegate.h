//
//  GameEffectDelegate.h
//  MAX
//
//  Created by Anton Katekov on 25.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GameEffectDelegate_h
#define MAX_GameEffectDelegate_h

#include "cocos2d.h"

using namespace cocos2d;

class GameEffect;

class GameEffectDelegate {
    
public:
    
    virtual void GameEffectDidFinishExistance(GameEffect *unit) = 0;
    
};

#endif