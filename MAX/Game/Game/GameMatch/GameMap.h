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
class GameUnit;

class GameMap {
    
    char* _groundType;//0-ground 1-water 2-coast 3-unpassable
    
public:
    
    //TODO: add trash after destroyed buildings
    
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
    GROUND_TYPE GroundTypeAtIndex(const int index) const;
    GROUND_TYPE GroundTypeAtXY(const int x, const int y) const;
};

#endif /* defined(__MAX__MAXMap__) */
