//
//  GameMap.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameMap__
#define __MAX__GameMap__

#include <iostream>
#include "miniPrefix.h"
#include "GameMapResources.h"

class Texture;
class MAXContentMap;

typedef enum
{
    GROUND_TYPE_GROUND,
    GROUND_TYPE_WATER,
    GROUND_TYPE_COAST,
    GROUND_TYPE_UNPASSABLE
} GROUND_TYPE;

class GameUnit;
class MAXContentMap;

class GameMap {
    
    GROUND_TYPE* _groundType;//0-ground 1-water 2-coast 3-unpassable
    
    USimpleContainer<GameUnit*> _units;
    
public:
    int _w;
    int _h;
    
    shared_ptr<MAXContentMap> _contentMap;
    
    int GetMapWidth() const {return _w;};
    int GetMapHeight() const {return _h;};
    
    Texture* minimap;
    Texture* mapElementsSingle;
    
    Texture* palette;
    
    GameMap(const shared_ptr<MAXContentMap>& map);
    ~GameMap();
    GROUND_TYPE GroundTypeAtPoint(const CCPoint& point) const;
    GROUND_TYPE GroundTypeAtXY(const int x, const int y) const;
};

#endif /* defined(__MAX__MAXMap__) */
