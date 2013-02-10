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

class Texture2D;

class GameMatchPlayer {
    
public:
    
    GameMatchPlayer();
    ~GameMatchPlayer();
    
    int _id;
    string _name;
    Color _playerColor;
    
    Texture2D* _palette;
};

#endif /* defined(__MAX__Game_MatchPlayer__) */
