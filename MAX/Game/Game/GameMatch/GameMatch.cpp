//
//  GameMatch.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatch.h"
#include "GameMatchPlayer.h"
#include "GameMap.h"
#include "MAXContetnLoader.h"
#include "MAXConfigManager.h"


GameMatch::GameMatch(string configName, string mapName)
{
    MAXConfigManager::SharedMAXConfigManager()->LoadConfigsFromFile(configName);
    
    shared_ptr<MAXContentMap> map1 = MAXSCL->LoadMapWithName(mapName);
    _map = new GameMap(map1);
}

GameMatch::~GameMatch()
{
    if (_map)
        delete _map;
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player = _players[i];
        delete player;
    }
}