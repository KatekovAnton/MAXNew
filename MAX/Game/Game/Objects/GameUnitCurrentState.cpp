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
:_unitParameters(params), _landed(false)
{
    for (int pt = UNIT_PARAMETER_TYPE_MIN; pt <= UNIT_PARAMETER_TYPE_MAX; pt++)
    {
        int val = GetMaxParameterValue((UNIT_PARAMETER_TYPE)pt);
        SetParameterValue((UNIT_PARAMETER_TYPE)pt, val);
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        _detected[i] = false;
    }
}

GameUnitCurrentState::~GameUnitCurrentState()
{
    delete _unitParameters;
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
        case UNIT_PARAMETER_TYPE_SHOTS:
            result = _pShots;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            result = _pFuel;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            result = _pAmmo;
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
            result = _unitParameters->_pMaxSpeed * 10;
            break;
        case UNIT_PARAMETER_TYPE_HEALTH:
            result = _unitParameters->_pMaxHealth;
            break;
        case UNIT_PARAMETER_TYPE_ARMOR:
            result = _unitParameters->_pMaxArmor;
            break;
        case UNIT_PARAMETER_TYPE_ATTACK:
            result = _unitParameters->_pMaxAttack;
            break;
        case UNIT_PARAMETER_TYPE_SHOTS:
            result = _unitParameters->_pMaxShots;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            result = _unitParameters->_pMaxFuel * 10;
            break;
        case UNIT_PARAMETER_TYPE_RANGE:
            result = _unitParameters->_pMaxRange;
            break;
        case UNIT_PARAMETER_TYPE_SCAN:
            result = _unitParameters->_pMaxScan;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            result = _unitParameters->_pMaxAmmo;
            break;
        case UNIT_PARAMETER_TYPE_COST:
            result = _unitParameters->_pMaxCost;
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
        case UNIT_PARAMETER_TYPE_SHOTS:
            _pShots = newValue;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            _pFuel = newValue;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            _pAmmo = newValue;
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

    // refuel only when no fuel
    parameterType = UNIT_PARAMETER_TYPE_GAS;
    val = GetParameterValue(parameterType);
    if (val <= 0)
    {
        val = GetMaxParameterValue(parameterType);
        SetParameterValue(parameterType, val);
    }
    
    // reload only when no ammo
    parameterType = UNIT_PARAMETER_TYPE_AMMO;
    val = GetParameterValue(parameterType);
    if (val <= 0)
    {
        val = GetMaxParameterValue(parameterType);
        SetParameterValue(parameterType, val);
    }
}

int GameUnitCurrentState::GetMoveBalance()
{
    int result = GetParameterValue(UNIT_PARAMETER_TYPE_SPEED);
    if (GetMaxParameterValue(UNIT_PARAMETER_TYPE_GAS) > 0)
    {
        int fuel  = GetParameterValue(UNIT_PARAMETER_TYPE_GAS);
        if (fuel < result)
        {
            result = fuel;
        }
    }
    return result;
}

void GameUnitCurrentState::MoveWithCost(const int cost)
{
    if (GetMoveBalance() < cost)
    {
        return;
    }
    
    int val;
    UNIT_PARAMETER_TYPE parameterType;

    parameterType = UNIT_PARAMETER_TYPE_SPEED;
    val = GetParameterValue(parameterType);
    val -= cost;
    if (val < 0)
    {
        val = 0;
    }
    SetParameterValue(parameterType, val);

    parameterType = UNIT_PARAMETER_TYPE_GAS;
    val = GetParameterValue(parameterType);
    val -= cost;
    if (val < 0)
    {
        val = 0;
    }
    SetParameterValue(parameterType, val);
    
    
    if (!_unitParameters->GetConfig()->_pMoveAndShot)
    {
        int maxShots = GetMaxParameterValue(UNIT_PARAMETER_TYPE_SHOTS);
        int maxSpeed = GetMaxParameterValue(UNIT_PARAMETER_TYPE_SPEED);
        if (maxShots > 0 && maxSpeed > 0)
        {
            float speedPerShot = maxSpeed / maxShots;
            int speed = GetParameterValue(UNIT_PARAMETER_TYPE_SPEED);
            
            parameterType = UNIT_PARAMETER_TYPE_SHOTS;
            val = GetParameterValue(parameterType);
            int shots = speed / speedPerShot;
            if (shots < val)
            {
                val = shots;
            }
            if (val < 0)
            {
                val = 0;
            }
            SetParameterValue(parameterType, val);
        }
    }
}

int GameUnitCurrentState::GetShotBalance()
{
    int result = GetParameterValue(UNIT_PARAMETER_TYPE_SHOTS);
    if (GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO) > 0)
    {
        int ammo  = GetParameterValue(UNIT_PARAMETER_TYPE_AMMO);
        if (ammo < result)
        {
            result = ammo;
        }
    }
    return result;
}

void GameUnitCurrentState::MakeShot()
{
    if (GetShotBalance() < 1)
    {
        return;
    }
    
    int val;
    UNIT_PARAMETER_TYPE parameterType;
    
    parameterType = UNIT_PARAMETER_TYPE_SHOTS;
    val = GetParameterValue(parameterType);
    val -= 1;
    if (val < 0)
    {
        val = 0;
    }
    SetParameterValue(parameterType, val);
    
    parameterType = UNIT_PARAMETER_TYPE_AMMO;
    val = GetParameterValue(parameterType);
    val -= 1;
    if (val < 0)
    {
        val = 0;
    }
    SetParameterValue(parameterType, val);
    
    if (!_unitParameters->GetConfig()->_pMoveAndShot)
    {
        int maxShots = GetMaxParameterValue(UNIT_PARAMETER_TYPE_SHOTS);
        int maxSpeed = GetMaxParameterValue(UNIT_PARAMETER_TYPE_SPEED);
        int speedPerShot = maxSpeed / maxShots;
        
        parameterType = UNIT_PARAMETER_TYPE_SPEED;
        val = GetParameterValue(parameterType);
        val -= speedPerShot;
        if (val < 0)
        {
            val = 0;
        }
        SetParameterValue(parameterType, val);
    }
}
