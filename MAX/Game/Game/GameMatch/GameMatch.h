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

class GameMatchPlayer;
class GameBattlefield;

class GameMatch {
    
public:
    
    GameBattlefield* _battlefield;
    vector<GameMatchPlayer*> players;

    GameMatch();
    ~GameMatch();
};

#endif /* defined(__MAX__Game_atch__) */
