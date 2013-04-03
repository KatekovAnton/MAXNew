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
    
    void Recount(GameUnit *unit, bool withIncreasing, const CCPoint &centerPoint);
    
    void Increase(const CCPoint &point);
    void Decrease(const CCPoint &point);    
    
    void Update(GameUnit *unit, const CCPoint &centerPoint);
    void Reset(GameUnit *unit, const CCPoint &centerPoint);
    
    void BeginUpdates();
    void EndUpdates();
    
public:
    
    GameFogDelegate *_delegate_w;
    
    
    bool IsInTouchZone(GameUnit *unit) const;
    int GetValue(const CCPoint &point) const;
    
    
    void UpdateOnUnitDidStartMove(GameUnit* unit);
    void UpdateOnUnitDidEndMove(GameUnit* unit);
    void UpdateOnUnitDidPlaceToMap(GameUnit* unit);
    void UpdateOnUnitDidRemoveFromMap(GameUnit* unit);
    
    GameFog(int mapWidth, int mapHeight);
    ~GameFog();
};

#endif /* defined(__MAX__GameFog__) */
