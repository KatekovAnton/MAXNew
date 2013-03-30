//
//  PlayerResourceMap.cpp
//  MAX
//
//  Created by  Developer on 30.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "PlayerResourceMap.h"

PlayerResourceMap::PlayerResourceMap(const int mapW, const int mapH)
:_mapW(mapW), _mapH(mapH), _resScan(new unsigned char[mapW * mapH])
{
    memset(_resScan, 0, mapW * mapH * sizeof(unsigned char));
}

PlayerResourceMap::~PlayerResourceMap()
{
    delete []_resScan;
}

void PlayerResourceMap::AddCell(const int x, const int y)
{
    _resScan[IndexOf(x, y)] = true;
}