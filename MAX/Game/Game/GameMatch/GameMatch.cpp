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


GameMatch::GameMatch(const string& configName, const string& mapName, const vector<GameMatchPlayerInfo>& players)
{
    MAXConfigManager::SharedMAXConfigManager()->LoadConfigsFromFile(configName);
    
    shared_ptr<MAXContentMap> map1 = MAXSCL->LoadMapWithName(mapName);
    _map = new GameMap(map1);
    
    for (int i = 0; i < players.size(); i++) {
        GameMatchPlayer* player = new GameMatchPlayer(players[i]);
        _players.push_back(player);
    }
    
    _currentPlayer_w = _players[0];
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