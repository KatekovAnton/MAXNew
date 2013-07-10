//
//  GameMap.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameMap.h"
#include "GameUnit.h"
#include "GameUnitParameters.h"
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
            _groundType[i * _w + j] = map->groundType[(int)element];
        }
    }
    _contentMap = map;
}

GameMap::~GameMap()
{
    free(_groundType);
	_contentMap = nullptr;
}

GROUND_TYPE GameMap::GroundTypeAtPoint(const CCPoint& point) const
{
    return (GROUND_TYPE)_groundType[(int)point.y * _w + (int)point.x];
}

GROUND_TYPE GameMap::GroundTypeAtIndex(const int index) const
{
    return (GROUND_TYPE)_groundType[index];
}

GROUND_TYPE GameMap::GroundTypeAtXY(const int x, const int y) const
{
    return (GROUND_TYPE)_groundType[y * _w + x];
}
