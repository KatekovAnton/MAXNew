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

GameMatch::GameMatch()
{}

GameMatch::~GameMatch()
{
    for (int i = 0; i < _players.size(); i++) {
        GameMatchPlayer* player = _players[i];
        delete player;
    }
}