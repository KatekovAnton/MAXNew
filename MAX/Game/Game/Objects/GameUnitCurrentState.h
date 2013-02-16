//
//  GameUnitCurrentState.h
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameUnitCurrentState__
#define __MAX__GameUnitCurrentState__

#include <iostream>

class GameUnitParameters;

class GameUnitCurrentState {
    
    GameUnitParameters* _wParams;
    
public:
    
    GameUnitCurrentState(GameUnitParameters* params);
    ~GameUnitCurrentState();
    
    void StartNewTurn();
    
};

#endif /* defined(__MAX__GameUnitCurrentState__) */
