//
//  GameUnitCurrentState.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitCurrentState.h"
#include "GameUnitParameters.h"
#include "MAXObjectConfig.h"

GameUnitCurrentState::GameUnitCurrentState(GameUnitParameters* params)
:_unitBaseParameters(params)
{
    for (UNIT_PARAMETER_TYPE pt = UNIT_PARAMETER_TYPE_MIN; pt <= UNIT_PARAMETER_TYPE_MAX; pt++)
    {
        int val = _unitBaseParameters->GetParameterValue(pt);
        SetParameterValue(pt, val);
    }
}

GameUnitCurrentState::~GameUnitCurrentState()
{
    delete _unitBaseParameters;
}

int GameUnitCurrentState::GetParameterValue(UNIT_PARAMETER_TYPE parameterType)
{
    int result = 0;
    switch (parameterType)
    {
        case UNIT_PARAMETER_TYPE_SPEED:
            result = _pSpeed;
            break;
        case UNIT_PARAMETER_TYPE_HEALTH:
            result = _pHealth;
            break;
        case UNIT_PARAMETER_TYPE_ARMOR:
            result = _pArmor;
            break;
        case UNIT_PARAMETER_TYPE_ATTACK:
            result = _pAttack;
            break;
        case UNIT_PARAMETER_TYPE_SHOTS:
            result = _pShots;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            result = _pFuel;
            break;
        case UNIT_PARAMETER_TYPE_RANGE:
            result = _pRange;
            break;
        case UNIT_PARAMETER_TYPE_SCAN:
            result = _pScan;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            result = _pAmmo;
            break;
        case UNIT_PARAMETER_TYPE_COST:
            result = _pCost;
            break;
            
        default:
            break;
    }
    return result;
}

int GameUnitCurrentState::GetMaxParameterValue(UNIT_PARAMETER_TYPE parameterType)
{
    int result = 0;
    switch (parameterType)
    {
        case UNIT_PARAMETER_TYPE_SPEED:
            result = _unitBaseParameters->_pMaxSpeed;
            break;
        case UNIT_PARAMETER_TYPE_HEALTH:
            result = _unitBaseParameters->_pMaxHealth;
            break;
        case UNIT_PARAMETER_TYPE_ARMOR:
            result = _unitBaseParameters->_pMaxArmor;
            break;
        case UNIT_PARAMETER_TYPE_ATTACK:
            result = _unitBaseParameters->_pMaxAttack;
            break;
        case UNIT_PARAMETER_TYPE_SHOTS:
            result = _unitBaseParameters->_pMaxShots;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            result = _unitBaseParameters->_pMaxFuel;
            break;
        case UNIT_PARAMETER_TYPE_RANGE:
            result = _unitBaseParameters->_pMaxRange;
            break;
        case UNIT_PARAMETER_TYPE_SCAN:
            result = _unitBaseParameters->_pMaxScan;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            result = _unitBaseParameters->_pMaxAmmo;
            break;
        case UNIT_PARAMETER_TYPE_COST:
            result = _unitBaseParameters->_pMaxCost;
            break;
        case UNIT_PARAMETER_TYPE_CARGO_PLANES:
        case UNIT_PARAMETER_TYPE_CARGO_SHIPS:
        case UNIT_PARAMETER_TYPE_CARGO_UNITS:
            result = 0;
            break;
            
        default:
            break;
    }
    return result;
}

void GameUnitCurrentState::SetParameterValue(UNIT_PARAMETER_TYPE parameterType, int newValue)
{
    switch (parameterType)
    {
        case UNIT_PARAMETER_TYPE_SPEED:
            _pSpeed = newValue;
            break;
        case UNIT_PARAMETER_TYPE_HEALTH:
            _pHealth = newValue;
            break;
        case UNIT_PARAMETER_TYPE_ARMOR:
            _pArmor = newValue;
            break;
        case UNIT_PARAMETER_TYPE_ATTACK:
            _pAttack = newValue;
            break;
        case UNIT_PARAMETER_TYPE_SHOTS:
            _pShots = newValue;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            _pFuel = newValue;
            break;
        case UNIT_PARAMETER_TYPE_RANGE:
            _pRange = newValue;
            break;
        case UNIT_PARAMETER_TYPE_SCAN:
            _pScan = newValue;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            _pAmmo = newValue;
            break;
        case UNIT_PARAMETER_TYPE_COST:
            _pCost = newValue;
            break;
            
        default:
            break;
    }
}

void GameUnitCurrentState::StartNewTurn()
{
    int val;
    UNIT_PARAMETER_TYPE parameterType;

    // temporary solution
    parameterType = UNIT_PARAMETER_TYPE_SPEED;
    val = GetMaxParameterValue(parameterType);
    SetParameterValue(parameterType, val);
    
    parameterType = UNIT_PARAMETER_TYPE_SHOTS;
    val = GetMaxParameterValue(parameterType);
    SetParameterValue(parameterType, val);
}
