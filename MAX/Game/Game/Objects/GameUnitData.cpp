//
//  GameUnitData.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitData.h"
#include "GUTask.h"
#include "GameUnitParameters.h"
#include "MAXObjectConfig.h"
#include "Geometry.h"

GameUnitData::GameUnitData(GameUnitParameters* params, int ownerId)
	:_unitParameters(params), _landed(false), _isPlacedOnMap(false), _disabledByInfiltrator(false), _currentTask(NULL), _isUnderConstruction(false), _isUniteractable(false), _ownerId(ownerId), _isPlacingMines(false), _isRemovingMines(false)
{
    _isOn = GetConfig()->_isBuilding && GetConfig()->_isAllwaysOn;
    
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

GameUnitData::~GameUnitData()
{
    delete _unitParameters;
    if (_currentTask) {
        _currentTask->AbortTask();
        delete _currentTask;
    }
}

void GameUnitData::SetIsUniteractable(bool value)
{
	_isUniteractable = value;
}

void GameUnitData::SetPlacingMines(bool action)
{
    _isRemovingMines = false;
    _isPlacingMines = GetConfig()->_isBombMinelayer && action;
}

void GameUnitData::SetRemovingMines(bool action)
{
    _isPlacingMines = false;
    _isRemovingMines = GetConfig()->_isBombMinelayer && action;
}

bool GameUnitData::GetIsSurvivor() const
{
    return _unitParameters->GetIsSurvivor();
}

bool GameUnitData::GetIsBuilding() const
{
    return _unitParameters->GetIsBuilding();
}

int GameUnitData::GetSize() const
{
    return _unitParameters->GetSize();
}

bool GameUnitData::GetIsAmphibious() const
{
    return GetConfig()->_isAmphibious;
}

bool GameUnitData::GetIsUnderwater() const
{
    return GetConfig()->_isUnderwater;
}

bool GameUnitData::GetIsBuldozer() const
{
    return GetConfig()->_isBuldozer;
}

bool GameUnitData::GetIsConnectored() const
{
    return GetConfig()->_isRetranslator || GetConfig()->_isConnector;
}

bool GameUnitData::CanMove() const
{
    return !GetIsBuilding() && _currentTask == NULL;
}

MAXObjectConfig* GameUnitData::GetConfig() const
{
    return _unitParameters->GetConfig();
}

bool GameUnitData::IsCellOfUnit(const CCPoint &p) const
{
    if (GetSize() == 1)
        return (int)p.x == (int)_unitCell.x && (int)p.y == (int)_unitCell.y;
    else
        return ((int)p.x == (int)_unitCell.x || (int)p.x == (int)_unitCell.x + 1) && ((int)p.y == (int)_unitCell.y || (int)p.y == (int)_unitCell.y + 1);
}

bool GameUnitData::GetCanStartBuildProcess() const
{
    return GetConfig()->_containProcessState;
}

bool GameUnitData::IsDetectedByPlayer(unsigned int playerId)
{
    return _detected[playerId];
    
//    bool result = false;
//    if (playerId < MAX_PLAYERS)
//    {
//        result = GetConfig()->_isStealthable && _detected[playerId];
//    }
//    return result;
}

vector<UNIT_MENU_ACTION> GameUnitData::GetActionList(bool havePath) const
{
	MAXObjectConfig* config = GetConfig();
    vector<UNIT_MENU_ACTION> result;
    
	result.push_back(UNIT_MENU_ACTION_INFO);
    if (config->_rFuel > 0 || config->_rMaterial > 0 || config->_rGold > 0) {
        result.push_back(UNIT_MENU_ACTION_XFER);
    }
	if (havePath)
	{
		result.push_back(UNIT_MENU_ACTION_DONE);
		result.push_back(UNIT_MENU_ACTION_STOP);
	}
	if (config->_isAbleToFire && (_pShots > 0))
	{
		result.push_back(UNIT_MENU_ACTION_ATTACK);
	}
	if (config->_isReloader) // check if resource exist
	{
		result.push_back(UNIT_MENU_ACTION_RELOAD);
	}
	if (config->_isRepair) // check if resource exist
	{
		result.push_back(UNIT_MENU_ACTION_REPAIR);
	}
	if (config->_isBuldozer) // check if trash exist under the unit
	{
		result.push_back(UNIT_MENU_ACTION_CLEAR);
	}
	if (config->_isResearch)
	{
		result.push_back(UNIT_MENU_ACTION_RESEARCH);
	}
	if (config->_isUpgrades)
	{
		result.push_back(UNIT_MENU_ACTION_BUYUPGRADES);
	}
	if (config->_isBuilding)
	{
        if (GetConfig()->_bSelfCreatorType != 0)
        {
            if (ContainsCurrentTask() && (!GetIsTaskPaused() && !GetIsTaskFinished()))
                result.push_back(UNIT_MENU_ACTION_STOP);
            else if (ContainsCurrentTask() && GetIsTaskPaused() && !GetIsTaskFinished())
                result.push_back(UNIT_MENU_ACTION_START);
        }
		else if (!GetConfig()->_isAllwaysOn)
		{
            if (_isOn)
                result.push_back(UNIT_MENU_ACTION_STOP);
            else
                result.push_back(UNIT_MENU_ACTION_START);
		}
	}
    else
    {
        if (GetConfig()->_bSelfCreatorType != 0)
        {
            if (ContainsCurrentTask() && (!GetIsTaskPaused() && !GetIsTaskFinished()))
                result.push_back(UNIT_MENU_ACTION_STOP);
        }
    }
    
    if (config->_isBombMinelayer) {
        result.push_back(UNIT_MENU_ACTION_PLACEMINES);
        result.push_back(UNIT_MENU_ACTION_REMOVEMINES);
    }
        
    if (config->_bSelfCreatorType != 0 && !ContainsCurrentTask())
	{
        result.push_back(UNIT_MENU_ACTION_BUILD);
	}
	if (config->_isInfiltrator && (_pShots > 0))
	{
		result.push_back(UNIT_MENU_ACTION_DISABLE);
		result.push_back(UNIT_MENU_ACTION_STEAL);
	}
    
    if ((config->_sGround != 0 && _storedUnits.size() != config->_sGround) ||
        (config->_sAir != 0 && _storedUnits.size() != config->_sAir) ||
        (config->_sSea != 0 && _storedUnits.size() != config->_sSea) ||
        (config->_sInfantry != 0 && _storedUnits.size() != config->_sInfantry))
	{
		result.push_back(UNIT_MENU_ACTION_LOAD);
		result.push_back(UNIT_MENU_ACTION_ACTIVATE);
	}
    
	if (config->_isBuilding)
	{
		result.push_back(UNIT_MENU_ACTION_REMOVE);
	}
    return result;
}

SOUND_TYPE GameUnitData::GetOnSelectSoundType() const
{
    if (GetConfig()->_isBuilding)
        return SOUND_TYPE_NONE;
    if (ContainsCurrentTask())
    {
        if (GetIsTaskFinished())
        {
            if (GetIsBuilding())
                return SOUND_TYPE_UNIT_COMPLETED;
            else
                return SOUND_TYPE_CONSTRUCTION_COMPLETE;
        }
        return SOUND_TYPE_BUILDING;
    }
    if (GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH) <= GetMaxParameterValue(UNIT_PARAMETER_TYPE_HEALTH) / 4)
        return SOUND_TYPE_STATUS_RED;
    if (GetParameterValue(UNIT_PARAMETER_TYPE_AMMO) == 0 && GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO) != 0)
        return SOUND_TYPE_AMMO_NONE;
    if (GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH) <= GetMaxParameterValue(UNIT_PARAMETER_TYPE_HEALTH) / 2)
        return SOUND_TYPE_STATUS_YELLOW;
    if (GetParameterValue(UNIT_PARAMETER_TYPE_AMMO) <= GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO) / 4 && GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO) != 0)
        return SOUND_TYPE_AMMO_LOW;
    if (GetParameterValue(UNIT_PARAMETER_TYPE_SPEED)/10 == 0)
        return SOUND_TYPE_MOVEMENT_EXHAUSTED;
    if (GetConfig()->_isSurvivor)
        return SOUND_TYPE_SURVEYING;
    if (GetIsPlacingMines())
        return SOUND_TYPE_LAYING_MINES;
    if (GetIsRemovingMines())
        return SOUND_TYPE_REMOVING_MINES;
    
    return SOUND_TYPE_READY;
}

UNIT_SOUND GameUnitData::GetBackgroundSoundType() const
{
    if (ContainsCurrentTask() && !GetIsTaskFinished() && !GetIsTaskPaused())
        return UNIT_SOUND_WORK;
    else
        return UNIT_SOUND_ENGINE;
}

int GameUnitData::GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const
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
            result = _pGas;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            result = _pAmmo;
            break;
        case UNIT_PARAMETER_TYPE_MATERIAL:
            result = _pRawMat;
            break;
        case UNIT_PARAMETER_TYPE_GOLD:
            result = _pGold;
            break;
        case UNIT_PARAMETER_TYPE_FUEL:
            result = _pFuel;
            break;
            
        default:
            break;
    }
    return result;
}

int GameUnitData::GetMaxParameterValue(UNIT_PARAMETER_TYPE parameterType) const
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
            
            //stored resources
        case UNIT_PARAMETER_TYPE_FUEL:
            result = GetConfig()->_rFuel;
            break;
        case UNIT_PARAMETER_TYPE_GOLD:
            result = GetConfig()->_rGold;
            break;
        case UNIT_PARAMETER_TYPE_MATERIAL:
            result = GetConfig()->_rMaterial;
            break;
            
            
        case UNIT_PARAMETER_TYPE_MATERIAL_BASE:
        {
            //TODO: calc at all base
            result = 0;
        }break;
            
        default:
            break;
    }
    return result;
}

int GameUnitData::GetDamageValue(int attack, int decrease) const
{
    attack = attack / decrease;
    int myArmor = GetParameterValue(UNIT_PARAMETER_TYPE_ARMOR);
    
    int resultDamage = attack - myArmor;
    if (resultDamage < 1)
        resultDamage = 1;
    return resultDamage;
}

void GameUnitData::SetParameterValue(UNIT_PARAMETER_TYPE parameterType, int newValue)
{
    switch (parameterType)
    {
        case UNIT_PARAMETER_TYPE_SPEED:
            _pSpeed = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_HEALTH:
            _pHealth = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_ARMOR:
            _pArmor = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_SHOTS:
            _pShots = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            _pGas = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            _pAmmo = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_MATERIAL:
            _pRawMat = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_GOLD:
            _pGold = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
        case UNIT_PARAMETER_TYPE_FUEL:
            _pFuel = ____min(newValue, GetMaxParameterValue(parameterType));
            break;
            
        default:
            break;
    }
}

void GameUnitData::StartNewTurn()
{
    int val;

	{
		// temporary solution
		val = GetMaxParameterValue(UNIT_PARAMETER_TYPE_SPEED);
		SetParameterValue(UNIT_PARAMETER_TYPE_SPEED, val);
    
		val = GetMaxParameterValue(UNIT_PARAMETER_TYPE_SHOTS);
		int ammoAble = GetParameterValue(UNIT_PARAMETER_TYPE_AMMO);
		if (val > ammoAble) 
			val = ammoAble;
		SetParameterValue(UNIT_PARAMETER_TYPE_SHOTS, val);
	}
    //TODO: remove refuel when no fuel
    val = GetParameterValue(UNIT_PARAMETER_TYPE_GAS);
    if (val <= 0)
    {
        val = GetMaxParameterValue(UNIT_PARAMETER_TYPE_GAS);
        SetParameterValue(UNIT_PARAMETER_TYPE_GAS, val);
    }
    
    if (_currentTask && !_paused && !_currentTask->IsFinished())
        _currentTask->UpdateOnStartTurn();
    
}

int GameUnitData::GetMoveBalance()
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

void GameUnitData::MoveWithCost(const int cost)
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

int GameUnitData::GetShotBalance()
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

void GameUnitData::MakeShot()
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

void GameUnitData::SetTask(GUTask *newTask)
{
    if (_currentTask) 
        throw "GameUnitData::SetTask(GUTask *newTask): try to start new task but another one already exist!!";
    
    _currentTask = newTask;
    _currentTask->StartTask();
    _paused = false;
}

void GameUnitData::AbortTask()
{
    if (!_currentTask)
        return;
    
    _currentTask->AbortTask();
    delete _currentTask;
    _currentTask = NULL;
}

void GameUnitData::PauseTask()
{
    _paused = true;
}

void GameUnitData::ContinuePausedTask()
{
    _paused = false;
}

bool GameUnitData::GetIsTaskFinished() const
{
    return _currentTask && _currentTask->IsFinished();
}

bool GameUnitData::GetIsTaskPaused() const
{
    return _currentTask && _paused;
}

GameUnit* GameUnitData::GetTaskSecondUnit()
{
    if (!_currentTask)
        return NULL;
    
    return _currentTask->GetSecondObject();
}

bool GameUnitData::GetIsTaskWaitForUserFinish()
{
    return _currentTask && _currentTask->IsFinished() && _currentTask->NeedUserInteractionToFinish();
}

void GameUnitData::CompletlyFinishTask()
{
    _currentTask->FinishTask();
    delete _currentTask;
    _currentTask = NULL;
    _paused = false;
}


bool GameUnitData::RearmWithUnit(GameUnitData* unit)
{
    if (unit->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL)>0 &&
        this->GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO)>0 &&
        this->GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO) != this->GetParameterValue(UNIT_PARAMETER_TYPE_AMMO))
    {
        this->SetParameterValue(UNIT_PARAMETER_TYPE_AMMO, this->GetMaxParameterValue(UNIT_PARAMETER_TYPE_AMMO));
        unit->SetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL, unit->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL) - 1);
        return true;
    }
    return false;
}

bool GameUnitData::RepairWithUnit(GameUnitData* unit)
{
    int needRepair = this->GetMaxParameterValue(UNIT_PARAMETER_TYPE_HEALTH) - this->GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH);
    int canRepair = unit->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL) / 1;
    int repair =  canRepair > needRepair? needRepair : canRepair;
    int spentMaterial = repair * 1;
    
    
    this->SetParameterValue(UNIT_PARAMETER_TYPE_HEALTH, this->GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH) + repair);
    unit->SetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL, unit->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL) - spentMaterial);
    return repair > 0;
}

bool GameUnitData::TransformResourcesFromUnit(GameUnitData* unit, int count)
{
    return false;
}

bool GameUnitData::ReceiveDamage(GameUnitData* unit, int decrase)
{
    int resultDamage = GetDamageValue(unit->GetMaxParameterValue(UNIT_PARAMETER_TYPE_ATTACK), decrase);
    int myHealth = GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH);
    myHealth -= resultDamage;
    
    if (myHealth < 0)
        myHealth = 0;
    
    SetParameterValue(UNIT_PARAMETER_TYPE_HEALTH, myHealth);
	return GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH) > 0;
}

void GameUnitData::OnCellChanged()
{
    if (ContainsCurrentTask()) {
        _currentTask->UnitDidChangeCell();
    }
}

#pragma mark - Raduis and BBs
// return some box for the object if the one is located in the point
BoundingBox GameUnitData::GetBoundingBox(const CCPoint &point, const float radius) const
{
    int unitSize = GetConfig()->_bSize;
    
    // count borders with size corrections
    return BoundingBoxMake(
                           point.x - radius + (unitSize - 1),
                           point.y - radius + (unitSize - 1),
                           point.x + radius + (unitSize > 1 ? (unitSize - 2) : 0),
                           point.y + radius + (unitSize > 1 ? (unitSize - 2) : 0)
                           );
}

// return some box for the object for the current location
BoundingBox GameUnitData::GetCurrentBoundingBox(const float radius) const
{
    return GetBoundingBox(_unitCell, radius);
}

// check if point is in radius around the object
bool GameUnitData::IsInRadius(const CCPoint &point, const float radius) const
{
    float centerX = _unitCell.x;
    float centerY = _unitCell.y;
    
    // find the center for big objects
    if (GetConfig()->_bSize > 1)
    {
        centerX += 0.5 * GetConfig()->_bSize;
        centerY += 0.5 * GetConfig()->_bSize;
    }
    
    float distance = (point.x - centerX) * (point.x - centerX)
    + (point.y - centerY) * (point.y - centerY);
    
    return (radius * radius) >= distance;
}

// check if point is in radius around the object
bool GameUnitData::IsInRadius(const CCPoint &point, const float radius, const CCPoint &currentCenter) const
{
    float centerX = currentCenter.x;
    float centerY = currentCenter.y;
    
    // find the center for big objects
    if (GetConfig()->_bSize > 1)
    {
        centerX += 0.5 * GetConfig()->_bSize - 0.5;
        centerY += 0.5 * GetConfig()->_bSize - 0.5;
    }
    
    float distance = (point.x - centerX) * (point.x - centerX)
    + (point.y - centerY) * (point.y - centerY);
    
    return (radius * radius) >= distance;
}

BoundingBox GameUnitData::GetScanBoundingBox(const CCPoint &centerPoint) const
{
    return GetBoundingBox(centerPoint, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN));
}

BoundingBox GameUnitData::GetScanBoundingBox() const
{
    return GetBoundingBox(_unitCell, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN));
}

BoundingBox GameUnitData::GetFireBoundingBox() const
{
    return GetBoundingBox(_unitCell, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_RANGE));
}

// check if point is in scan radius around the object
bool GameUnitData::IsInScanRadius(const CCPoint &point) const
{
    return IsInRadius(point, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN));
}

// check if point is in scan radius around the object
bool GameUnitData::IsInScanRadius(const CCPoint &point, const CCPoint &currentCenter) const
{
    return IsInRadius(point, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN), currentCenter);
}

// check if point is in fire radius around the object
bool GameUnitData::IsInFireRadius(const CCPoint &point) const
{
    return IsInRadius(point, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_RANGE));
}

// check if point is in fire radius around the object
bool GameUnitData::IsInFireRadius(const CCPoint &point, const CCPoint &currentCenter) const
{
    return IsInRadius(point, _unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_RANGE), currentCenter);
}


