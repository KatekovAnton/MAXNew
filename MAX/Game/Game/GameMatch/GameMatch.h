//
//  GameМatch.h
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Game_atch__
#define __MAX__Game_atch__

#include <iostream>
#include "miniPrefix.h"
#include "GameMatchPlayer.h"

class GameMap;
class GameMapResources;
class GameSettings;
class GameMatchPlayer;
class Pathfinder;

class GameMatch {

    void DebugLandPlayer(GameMatchPlayer* player, const int i);
public:
    
    GameSettings*               _gameSettings;
    GameMap*                    _map;
    GameMapResources*           _resources;
    Pathfinder*                 _pathfinder;
    vector<GameMatchPlayer*>    _players;
    GameMatchPlayer*            _currentPlayer_w;

    GameMatch(const string& configName, const string& mapName, const vector<GameMatchPlayerInfo>& players);
    ~GameMatch();
};

#endif /* defined(__MAX__Game_atch__) */
