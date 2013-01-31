//
//  MAXMap.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXMap__
#define __MAX__MAXMap__

#include <iostream>
#include "miniPrefix.h"

class Texture;
class MAXContentMap;

enum MAXMAPGROUNDTYPE
{
    MAXMAPGROUNDTYPE_GROUND,
    MAXMAPGROUNDTYPE_WATER,
    MAXMAPGROUNDTYPE_COAST,
    MAXMAPGROUNDTYPE_UNPASSABLE
};

class MAXMap {
    
    char* _groundType;//0-ground 1-water 2-coast 3-unpassable
    int _w;
    int _h;
public:
    
    int GetMapWidth() const {return _w;};
    int GetMapHeight() const {return _h;};
    
    Texture* minimap;
    Texture* mapElementsSingle;
    
    Texture* palette;
    
    MAXMap(const shared_ptr<MAXContentMap>& map);
    ~MAXMap();
    char GroundTypeAtPoint(const CCPoint& point);
};

#endif /* defined(__MAX__MAXMap__) */
