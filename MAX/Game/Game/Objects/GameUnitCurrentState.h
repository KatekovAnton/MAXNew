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
#include "miniPrefix.h"

class GameUnitParameters;

class GameUnitCurrentState {
    
    
public:
    
    GameUnitParameters* _unitBaseParameters;
    
    int _pSpeed;
    int _pHealth;
    int _pArmor;
    int _pShots;
    int _pFuel;
    int _pAmmo;
    bool _landed;
    
    GameUnitCurrentState(GameUnitParameters* params);
    ~GameUnitCurrentState();

    int GetParameterValue(UNIT_PARAMETER_TYPE parameterType);
    int GetMaxParameterValue(UNIT_PARAMETER_TYPE parameterType);
    void SetParameterValue(UNIT_PARAMETER_TYPE parameterType, int newValue);
    
    void StartNewTurn();
    void MoveWithCost(const int cost);
};

#endif /* defined(__MAX__GameUnitCurrentState__) */
