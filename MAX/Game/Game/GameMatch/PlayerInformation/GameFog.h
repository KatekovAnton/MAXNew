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
#include "Geometry.h"

using namespace cocos2d;

class GameMap;
class GameUnit;
class GameFogDelegate;

class GameFog {
    int* _gameField;
    int* _gameFieldLast;
    BoundingBox _updatingBox;
    bool _complexUpdate;
    
    int _mapWidth;
    int _mapHeight;
    int _mapSize;
    
    int IndexOf(const CCPoint &point) const { return (int) _mapWidth * (int)point.y + (int)point.x; };
    int IndexOf(const int x, const int y) const { return  _mapWidth * y + x; };
    
    void Recount(GameUnit *unit, bool withIncreasing);
    
    void Increase(const CCPoint &point);
    void Decrease(const CCPoint &point);
    
    
public:
    
    GameFogDelegate *_delegate_w;
    
    void Update(GameUnit *unit);
    void Reset(GameUnit *unit);
    
    bool IsInTouchZone(GameUnit *unit) const;
    int GetValue(const CCPoint &point) const;
    
    void BeginUpdates();
    void EndUpdates();
    
    void UpdateWithUnitMove(GameUnit* unit, const CCPoint &movementPoint);
    
    GameFog(int mapWidth, int mapHeight);
    ~GameFog();
};

#endif /* defined(__MAX__GameFog__) */
