//
//  GameMap.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameMap.h"
#include "MAXContentMap.h"

GameMap::GameMap(const shared_ptr<MAXContentMap>& map)
{
    _w = map->w;
    _h = map->h;
    _groundType = (char*)malloc(_w * _h);
    for (int i = 0; i < _h; i++)
    {
        for (int j = 0; j < _w; j++)
        {
            short element = map->map[i * _w + j];
            char grundtype = map->groundType[(int)element];
            _groundType[i * _w + j] = grundtype;
        }
    }
}

GameMap::~GameMap()
{
    free(_groundType);
}

char GameMap::GroundTypeAtPoint(const CCPoint& point)
{
    return _groundType[(int)point.y * _w + (int)point.x];
}