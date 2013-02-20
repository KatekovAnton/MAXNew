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

class GameMap;
class GameMatchPlayer;

class GameMatch {
    
public:
    
    GameMap*                    _map;
    vector<GameMatchPlayer*>    _players;
    GameMatchPlayer*            _me;

    GameMatch(string configName, string mapName);
    ~GameMatch();
};

#endif /* defined(__MAX__Game_atch__) */
