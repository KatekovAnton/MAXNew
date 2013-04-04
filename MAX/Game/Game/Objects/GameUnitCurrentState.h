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
    
    
public:
    
    GameUnitParameters* _unitBaseParameters;
    
    int _pAttack;
    int _pSpeed;
    int _pHealth;
    int _pArmor;
    int _pShots;
    int _pFuel;
    int _pRange;
    int _pScan;
    int _pAmmo;
    int _pCost;
    
    GameUnitCurrentState(GameUnitParameters* params);
    ~GameUnitCurrentState();
    
    void StartNewTurn();
    
};

#endif /* defined(__MAX__GameUnitCurrentState__) */
