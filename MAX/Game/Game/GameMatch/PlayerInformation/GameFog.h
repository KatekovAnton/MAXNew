//
//  GameFog.h
//  MAX
//
//  Created by Eugene Fominykh on 3/24/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameFog__
#define __MAX__GameFog__

#include "cocos2d.h"

using namespace cocos2d;

class GameMap;
class GameUnit;

class GameFog {
    int* _gameField;
    
    int _mapWidth;
    int _mapHeight;
    int _mapSize;
    
    int IndexOf(CCPoint *point);
    
    void Recount(GameUnit *unit, CCPoint unitPosition, bool withIncreasing);
    
    void Increase(CCPoint *point);
    void Decrease(CCPoint *point);
    
public:
    void Update(GameUnit *unit, CCPoint unitPosition);
    void Reset(GameUnit *unit, CCPoint unitPosition);
    
    bool IsInTouchZone(GameUnit *unit);
    int GetValue(CCPoint *point);
    
    GameFog(int mapWidth, int mapHeight);
    ~GameFog();
};

#endif /* defined(__MAX__GameFog__) */
