//
//  GameMatchPlayer.h
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Game_MatchPlayer__
#define __MAX__Game_MatchPlayer__

#include <iostream>
#include "miniPrefix.h"
#include "MAXContetnConstants.h"


class Texture;
class GameUnit;
class PlayerBase;

struct GameMatchPlayerInfo {
    unsigned int    _playerId;
    string          _name;
    Color           _color;
};

class GameMatchPlayer {
    
public:
    
    GameMatchPlayerInfo _playerInfo;
    Texture* _palette;
    
    USimpleContainer<GameUnit*> _units;
    PlayerBase* _base;
    
    GameMatchPlayer(GameMatchPlayerInfo playerInfo);
    ~GameMatchPlayer();

    
    GameUnit* CreateUnit (int posx, int posy, string type, unsigned int ID);
    
};

#endif /* defined(__MAX__Game_MatchPlayer__) */
