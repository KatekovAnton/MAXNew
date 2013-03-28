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
    
    int IndexOf(const CCPoint &point) const { return (int) _mapWidth * (int)point.y + (int)point.x; };
    
    void Recount(GameUnit *unit, bool withIncreasing);
    
    void Increase(const CCPoint &point);
    void Decrease(const CCPoint &point);
    
public:
    void Update(GameUnit *unit);
    void Reset(GameUnit *unit);
    
    bool IsInTouchZone(GameUnit *unit) const;
    int GetValue(const CCPoint &point) const;
    
    GameFog(int mapWidth, int mapHeight);
    ~GameFog();
};

#endif /* defined(__MAX__GameFog__) */
