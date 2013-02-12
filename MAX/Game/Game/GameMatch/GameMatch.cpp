//
//  GameMatch.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatch.h"
#include "GameMatchPlayer.h"
#include "GameBattlefield.h"

GameMatch::GameMatch()
:_battlefield(NULL)
{}

GameMatch::~GameMatch()
{
    for (int i = 0; i < players.size(); i++) {
        GameMatchPlayer* player = players[i];
        delete player;
    }
    if(_battlefield)
        delete _battlefield;
}