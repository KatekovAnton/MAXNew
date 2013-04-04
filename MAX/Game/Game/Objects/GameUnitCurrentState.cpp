//
//  GameUnitCurrentState.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitCurrentState.h"
#include "GameUnitParameters.h"

GameUnitCurrentState::GameUnitCurrentState(GameUnitParameters* params)
:_unitBaseParameters(params)
{}

GameUnitCurrentState::~GameUnitCurrentState()
{
    delete _unitBaseParameters;
}

void GameUnitCurrentState::StartNewTurn()
{}