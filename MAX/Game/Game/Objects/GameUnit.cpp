//
//  Unit.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameUnit.h"
#include "Geometry.h"
#include "MyRandom.h"
#include "MAXGame.h"
#include "MAXUnitObject.h"
#include "MAXAnimationPrefix.h"
#include "MAXEngine.h"
#include "MAXObjectConfig.h"
#include "MAXConfigManager.h"

#include "SoundEngine.h"

#include "GameUnitData.h"
#include "GameMatchPlayer.h"
#include "GameUnitParameters.h"
#include "GameEffect.h"
#include "GameMatch.h"
#include "GameMap.h"
#include "PFWaveCell.h"
#include "MatchMapAgregator.h"

using namespace cocos2d;

MAXANIMATION_CURVE GetCurveForStep(const int step, const int pathSize)
{
    MAXANIMATION_CURVE curve;
    if (pathSize == 1)
        curve = MAXANIMATION_CURVE_EASE_IN_OUT;
    else if (step == pathSize - 1)
        curve = MAXANIMATION_CURVE_EASE_IN;
    else if (step == 0)
        curve = MAXANIMATION_CURVE_EASE_OUT;
    else
        curve = MAXANIMATION_CURVE_EASE_LINEAR;
    return curve;
}

GameUnit::GameUnit(MAXUnitObject* unitObject, GameUnitParameters* params)
:GameObject(unitObject, params->GetConfig()), _currentTopAnimation(NULL), _unitData(new GameUnitData(params)), _effectUnder(NULL), _effectUnderBuildingTape(NULL), _delegate_w(NULL), pathIndex(0), pathIsTemp(true), _currentTask(NULL)
{
    unitObject->_delegate_w = this;
    MAXObjectConfig* config = _unitData->GetConfig();
    unitObject->_needShadow = !config->_isUnderwater;
    _onDraw = false;
    if (_unitData->_isInProcess)
        CheckBodyAndShadow();
    
    
    currentSound = -1;
    workSound = -1;
    
    if(_unitData->GetIsBuilding() && _unitData->GetConfig()->_isNeedUndercover)
    {
        _effectUnder = GameEffect::CreateBuildingBase(_unitData->GetSize() == 2?BUILDING_BASE_TYPE_LARGE:BUILDING_BASE_TYPE_SMALL, OBJECT_LEVEL_ONGROUND);
    }
    ChackForAnimanteBody();
}

bool GameUnit::GetIsStealthable() const
{
    return _config_w->_isStealthable;
}

GameUnit::~GameUnit()
{
    delete _unitData;

    if (_effectUnder)
    {
        _effectUnder->Hide();
        delete _effectUnder;
    }
    
    if (_effectUnderBuildingTape)
    {
        _effectUnderBuildingTape->Hide();
        delete _effectUnderBuildingTape;
    }
}

void GameUnit::ChackForAnimanteBody()
{
    _shouldAnimateBody = _unitData->GetConfig()->bodyActiveFrame0 != _unitData->GetConfig()->bodyActiveFrame1 && !_unitData->_disabledByInfiltrator;
}

int GameUnit::PlaySound(UNIT_SOUND unitSound)
{
    MAXObjectConfig* config = _unitData->GetConfig();
    string* soundStr = NULL;
    bool loop = false;
    SoundEngineDelegate* delegate = NULL;
    
    if (config->_isShip)
    {
        if (unitSound == UNIT_SOUND_ENGINE)            unitSound = UNIT_SOUND_ENGINE_WATER;
        else if (unitSound == UNIT_SOUND_ENGINE_START) unitSound = UNIT_SOUND_ENGINE_START_WATER;
        else if (unitSound == UNIT_SOUND_ENGINE_STOP)  unitSound = UNIT_SOUND_ENGINE_STOP_WATER;
    }
    float vol = 1.0;
    float engVol = 0.5;
  
    switch (unitSound)
    {
        case UNIT_SOUND_BLAST:
            soundStr = &(config->_soundBlastName);
            break;
        case UNIT_SOUND_SHOT:
            soundStr = &(config->_soundShotName);
            break;
            
        case UNIT_SOUND_ENGINE:
            vol = engVol;
            soundStr = &(config->_soundEngineName);
            loop = true;
            break;
        case UNIT_SOUND_ENGINE_START:
            vol = engVol;
            soundStr = &(config->_soundEngineStartName);
            break;
        case UNIT_SOUND_ENGINE_STOP:
            vol = engVol;
            soundStr = &(config->_soundEngineStopName);
            break;
        case UNIT_SOUND_ENGINE_WATER:
            vol = engVol;
            soundStr = &(config->_soundEngineWaterName);
            loop = true;
            break;
        case UNIT_SOUND_ENGINE_START_WATER:
            vol = engVol;
            soundStr = &(config->_soundEngineStartWaterName);
            break;
        case UNIT_SOUND_ENGINE_STOP_WATER:
            vol = engVol;
            soundStr = &(config->_soundEngineStopWaterName);
            break;
            
        case UNIT_SOUND_START:
            soundStr = &(config->_soundStartName);
            break;
        case UNIT_SOUND_STOP:
            soundStr = &(config->_soundStopName);
            break;
        case UNIT_SOUND_WORK:
            soundStr = &(config->_soundWorkName);
            loop = true;
            break;
        case UNIT_SOUND_BUILD:
            soundStr = &(config->_soundBuildName);
            break;
    }
    int soundId = -1;
    if (soundStr->length() > 2)
    {
        if (*soundStr == "attack1.wav" || *soundStr == "aplan1.wav") {
            vol = 0.15;
        }
        soundId = SOUND->PlayGameSound(*soundStr, delegate, loop, vol);
    }
    return soundId;
}

void GameUnit::StopCurrentSound()
{
    if (currentSound > 0)
    {
        SOUND->StopGameSound(currentSound);
        currentSound = -1;
    }
}

void GameUnit::StopWorkSound()
{
    if (workSound > 0)
    {
        SOUND->StopGameSound(workSound);
        workSound = -1;
    }
}

void GameUnit::UnitDidSelect()
{
    StopCurrentSound();
    if (_owner_w->GetIsCurrentPlayer() && !_unitData->GetIsBuilding())
        SOUND->PlaySystemSound(SOUND_TYPE_READY); // SOUND_TYPE_UNIT_READY
    
    MAXObjectConfig* config = _unitData->GetConfig();
    if ((!config->_isBuilding) && (config->_bSelfCreatorType != 0 || config->_isBuldozer) && IsInProcess())
    {
        currentSound = PlaySound(UNIT_SOUND_WORK);
    }
    else
    {
        currentSound = PlaySound(UNIT_SOUND_ENGINE);
    }
}

void GameUnit::UnitDidDeselect()
{
    StopCurrentSound();
}

void GameUnit::SetDirection(int dir)
{
    if(_unitData->GetIsBuilding())
        return;
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->SetBodyDirection(dir);
    _unitObject->SetHeadDirection(dir);
    
    _unitData->_bodyDirection = dir;
    _unitData->_headDirection = dir;
}

void GameUnit::SetRandomDirection()
{
    if(_unitData->GetIsBuilding())
        return;
    SetDirection(nextIntMax(8));
}

void GameUnit::SetColor(GLKVector4 color)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->playerColor = color;
}

void GameUnit::Show()
{
    if (_onDraw)
        return;
    GameObject::Show();
    if (_effectUnder)
        _effectUnder->Show();
    CheckBodyAndShadow();
}

void GameUnit::Hide()
{
    StopCurrentSound();
    StopWorkSound();
    
    if (!_onDraw)
        return;
    
    GameObject::Hide();
    if (_effectUnder)
        _effectUnder->Hide();
}

void GameUnit::LiftBridge()
{
    GetUnitObject()->LiftBridge();
}

void GameUnit::DropBridge()
{
    GetUnitObject()->DropBridge();
}

void GameUnit::TakeOff()
{
    if (_unitData->_landed)
    {
        _unitData->_landed = false;
        GetUnitObject()->TakeOff();

        StopWorkSound();
        workSound = PlaySound(UNIT_SOUND_START);
    }
}

void GameUnit::Landing()
{
    if (!_unitData->_landed)
    {
        _unitData->_landed = true;
        GetUnitObject()->Landing();
        
        StopWorkSound();
        workSound = PlaySound(UNIT_SOUND_STOP);
    }
}

void GameUnit::LiftBridgeInstantly()
{
    GetUnitObject()->LiftBridgeInstantly();
}

void GameUnit::LandInstantly()
{
    if (!_unitData->_landed)
    {
        _unitData->_landed = true;
        GetUnitObject()->LandInstantly();
    }
}

void GameUnit::EscapeToLocation(const int x, const int y, const int cost)
{
    movePath.clear();
    pathIndex = 0;
    pathIsTemp = true;

    _unitData->MoveWithCost(cost);

    CCPoint destination = CCPointMake(x, y);
    int direction = MAXObject::CalculateImageIndex(_unitCell, destination);
    _delegate_w->GameUnitWillLeaveCell(this);
    SetLocation(destination);
    _delegate_w->GameUnitDidEnterCell(this);
    
    SetDirection(direction);
}

void GameUnit::NewTurn()
{
    StopCurrentSound();
    StopWorkSound();
    bool processed = false;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (_unitData->_detected[i])
        {
            _unitData->_detected[i] = false;
            if (!processed)
            {
                processed = true;
                GetUnitObject()->StealthActivated();
                _delegate_w->GameUnitDidUndetected(this);
            }
        }
    }
    _unitData->StartNewTurn();
}

void GameUnit::PlaceUnitOnMap()
{
    if (_unitData->_isPlacedOnMap)
        return;
    _unitData->_isPlacedOnMap = true;
    Show();
    // update the fog of war for the current gamer
    _delegate_w->GameUnitDidPlaceOnMap(this);
}

void GameUnit::RemoveUnitFromMap()
{
    if (!_unitData->_isPlacedOnMap)
        return;
    _unitData->_isPlacedOnMap = false;
    Hide();
    // update the fog of war for the current gamer
    _delegate_w->GameUnitDidRemoveFromMap(this);
}

void GameUnit::SetLocation(const cocos2d::CCPoint &cell)
{
    GameObject::SetLocation(cell);
    movePath.clear();
    _unitData->_unitCell = cell;
    pathIndex = 0;
    if (_effectUnder) 
        _effectUnder->SetLocation(_unitCell);
}

void GameUnit::CheckBodyAndShadow()
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if (!(_unitData->GetIsAmphibious() || _unitData->GetIsUnderwater() || CanStartBuildProcess()) && _unitData->GetIsBuilding())
    {
        //all passive-worked buildings, which cannot be topped by infiltrator
        if (_unitData->GetConfig()->_isAllwaysOn && _unitData->GetIsBuilding())
        {
            if (_unitData->GetSize() == 1)
                _unitObject->SetBodyOffset(0);//radar
            else
                _unitObject->SetBodyOffset(_unitData->GetConfig()->bodyIdleFrame0);//hangar
        }
        return;
    };
    
    
    EXTENDED_GROUND_TYPE groundType = game->_match->_agregator->GroundTypeAtXY(_unitCell.x, _unitCell.y);
    if (_unitData->GetConfig()->_isBuilding)
    {
        _unitObject->SetBodyOffset((IsInProcess() && (!_unitData->GetConfig()->_isAllwaysOn || !_unitData->GetConfig()->_isActiveBody))?1:0);
    }
    else
    {
        if (groundType == EXTENDED_GROUND_TYPE_WATER)
        {
            _unitObject->_currentLevel = OBJECT_LEVEL_ONGROUND;
            if (_unitData->GetConfig()->_isUnderwater)
            {
                _unitObject->SetBodyOffset(IsDetectedByPlayer(game->_match->_currentPlayer_w->_playerInfo._playerId)?8:0);
                _unitObject->_needShadow = false;
                return;
            }
            if (_unitData->GetIsAmphibious())
            {
                _unitObject->SetBodyOffset(IsInProcess()?24:8);
                return;
            }
        }
        else
        {
            _unitObject->_currentLevel = _unitObject->params_w->_bLevel;
            if (_unitData->GetIsUnderwater())
            {
                _unitObject->SetBodyOffset(8);
                _unitObject->_needShadow = _unitData->GetConfig()->_haveShadow;
                return;
            }
            if (_unitData->GetIsAmphibious())
            {
                _unitObject->SetBodyOffset(IsInProcess()?16:0);
                return;
            }
            if (_unitData->GetIsBuldozer())
            {
                _unitObject->SetBodyOffset(IsInProcess()?8:0);
                return;
            }
        }
    }
    
}

void GameUnit::SetPath(std::vector<PFWaveCell*> path)
{
    movePath = path;
    pathIsTemp = true;
	if (movePath.size() > 1)
	{
        pathIndex = movePath.size() - 2; // last value is current position
	}
	else
	{
		movePath.clear();
        pathIndex = 0;
	}
}

void GameUnit::ClearPath()
{
	movePath.clear();
	pathIndex = 0;
	pathIsTemp = true;
}

void GameUnit::ClearTempPath()
{
    if (pathIsTemp)
    {
        movePath.clear();
    }
}

std::vector<PFWaveCell*> GameUnit::GetPath()
{
	return movePath;
}

int GameUnit::GetPathIndex()
{
    return pathIndex;
}

PFWaveCell* GameUnit::GetNextPathCell()
{
    PFWaveCell* result = NULL;
    
    int pi = pathIndex;
    if (pi >= 0)
    {
        result = movePath[pi];
    }
    
    return result;
}

bool GameUnit::IsPathTargetedTo(const int x, const int y)
{
	bool result = FALSE;
	if (movePath.size() > 0)
	{
		PFWaveCell* cell = movePath[0];
		if (cell && (cell->x == x) && (cell->y == y))
		{
			result = TRUE;
		}
	}
	return result;
}

void GameUnit::ConfirmCurrentPath()
{
	if (movePath.size() > 1)
	{
		if (selectedGameObjectDelegate)
			selectedGameObjectDelegate->onUnitMoveStart(this);

        pathIsTemp = false;
        if (_unitData->GetConfig()->_isPlane)
        {
            TakeOff();
        }
		FollowPath();
	}
}

void GameUnit::AbortCurrentPath()
{
    if (_currentTopAnimation)
    {
        _currentTopAnimation->Stop();
    }
}

void GameUnit::FollowPath(void)
{
    MAXObjectConfig* config = _unitData->GetConfig();

    MAXUnitObject* _unitObject = GetUnitObject();
    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    sequence->_delegate = this;
    CCPoint pos = _unitCell;
    int bodyIndex = _unitObject->GetBodyIndex();
    int pi = pathIndex;
    bool first = true;
    int speed = _unitData->GetMoveBalance();
    MAXAnimationObjectUnit* move = NULL;
    while (pi >= 0)
    {
        PFWaveCell* cell = movePath[pi];
        movePathIndex = pi;
        
        speed -= cell->cost;
        if (speed < 0)
        {
            if (move)
            {
                MAXANIMATION_CURVE curve = GetCurveForStep(0, pathIndex - pi);
                move->_moveCurve = curve;
				PFWaveCell* cell2 = movePath[pi + 1];
                float speedMult = config->_pSpeed;
                if (config->_isPlane) 
                    speedMult = 18;
                
				float moveFactor = cell2->cost * 10.0 / speedMult; // change to current max speed
				move->SetMoveFactor(moveFactor);
            }
            break;
        }
        
        CCPoint destination = CCPointMake(cell->x, cell->y);
        int neededBodyIndex = MAXObject::CalculateImageIndex(pos, destination);
        if (neededBodyIndex != bodyIndex)
        {
            MAXAnimationObjectUnit* turn = new MAXAnimationObjectUnit(bodyIndex, neededBodyIndex, _unitObject->GetPureHeadIndex(), _unitObject);
            if (!first) // speed up unit rotation in middle of path
                turn->_aniTime = 0.01;
            sequence->AddAnimation(turn);
            bodyIndex = neededBodyIndex;
        }
        int pathSize = movePath.size() - 1;
        MAXANIMATION_CURVE curve = GetCurveForStep(pi, pathSize);
        move = new MAXAnimationObjectUnit(pos ,destination, _unitObject, curve);
        move->_delegate = this;
        float speedMult = config->_pSpeed;
        if (config->_isPlane)
            speedMult = 18;
        float moveFactor = cell->cost * 10.0 / speedMult; // change to current max speed
        move->SetMoveFactor(moveFactor);
        sequence->AddAnimation(move);
        
        if (first)
        {
            StopCurrentSound();
            currentSound = PlaySound(UNIT_SOUND_ENGINE_START);
        }
        
        pos = destination;
        pi--;
        first = false;
    }
    _currentTopAnimation = sequence;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
}

bool GameUnit::MoveToNextCell(void)
{
    bool result = false;
    if (pathIndex >= 0)
    {
        PFWaveCell* cell = movePath[pathIndex];
        SetUnitLocationAnimated(CCPointMake(cell->x, cell->y));
        pathIndex--;
    }
    return result;
}

void GameUnit::SetUnitLocationAnimated(const cocos2d::CCPoint &destination)
{
    if (_currentTopAnimation) 
        return;
    
    MAXUnitObject* _unitObject = GetUnitObject();
    int neededBodyIndex = MAXObject::CalculateImageIndex(_unitCell, destination);
    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    sequence->_delegate = this;
    if (neededBodyIndex != _unitObject->GetBodyIndex())
    {
        MAXAnimationObjectUnit* step1 = new MAXAnimationObjectUnit(_unitObject->GetBodyIndex(), neededBodyIndex, _unitObject->GetPureHeadIndex(), _unitObject);
        sequence->AddAnimation(step1);
    }
    MAXANIMATION_CURVE curve;
    int pathSize = movePath.size();
    if (pathSize == 2)
        curve = MAXANIMATION_CURVE_EASE_IN_OUT;
    else if (pathIndex == pathSize - 2)
        curve = MAXANIMATION_CURVE_EASE_IN;
    else if (pathIndex == 0)
        curve = MAXANIMATION_CURVE_EASE_OUT;
    else
        curve = MAXANIMATION_CURVE_EASE_LINEAR;
    
    MAXAnimationObjectUnit* step2 = new MAXAnimationObjectUnit(_unitCell ,destination, _unitObject, curve);
    step2->_delegate = this;
    sequence->AddAnimation(step2);
    
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
    _currentTopAnimation = sequence;
}

BoundingBox GameUnit::GetScanBoundingBox(const CCPoint &centerPoint) const
{
    return GetBoundingBox(centerPoint, _unitData->_unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN));
}

BoundingBox GameUnit::GetScanBoundingBox() const
{
    return GetBoundingBox(GetUnitCell(), _unitData->_unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN));
}

// check if point is in scan radius around the object
bool GameUnit::IsInScanRadius(const CCPoint &point) const
{
    return IsInRadius(point, _unitData->_unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN));
}

// check if point is in scan radius around the object
bool GameUnit::IsInScanRadius(const CCPoint &point, const CCPoint &currentCenter) const
{
    return IsInRadius(point, _unitData->_unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_SCAN), currentCenter);
}

// check if point is in fire radius around the object
bool GameUnit::IsInFireRadius(const CCPoint &point) const
{
    return IsInRadius(point, _unitData->_unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_RANGE));
}

// check if point is in fire radius around the object
bool GameUnit::IsInFireRadius(const CCPoint &point, const CCPoint &currentCenter) const
{
    return IsInRadius(point, _unitData->_unitParameters->GetParameterValue(UNIT_PARAMETER_TYPE_RANGE), currentCenter);
}

#pragma mark - Connectors

void GameUnit::UpdateConnectors()
{
    MAXUnitObject *object = GetUnitObject();
    object->RemoveConnectors();
    if (_unitData->GetSize() == 1)
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR4);
        
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR2);
       
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR1);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR3);
    }
    else
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR4);
        cell = ccp(GetUnitCell().x-1, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR8);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR3);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR7);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR1);
        
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR5);
        
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR2);
        
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR6);
    }
}

vector<CCPoint> GameUnit::GetNerbyCells() const
{
    vector<CCPoint> resuplt;
    if (_unitData->GetSize() == 1)
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
    }
    else
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x-1, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
    }
    return resuplt;
}

#pragma mark - Stealth methods

void GameUnit::DetectedByPlayer(unsigned int playerId)
{
    if (playerId < MAX_PLAYERS)
    {
        if (_config_w->_isStealthable && !_unitData->_detected[playerId])
        {
            _unitData->_detected[playerId] = true;
            _unitData->_detected[_owner_w->_playerInfo._playerId] = true;
            if (game->_match->GetIsCurrentPlayer(playerId) ||
                game->_match->GetIsCurrentPlayer(_owner_w->_playerInfo._playerId))
            {
                GetUnitObject()->StealthDeactivated();
                _delegate_w->GameUnitDidDetected(this);
            }
            if (game->_match->GetIsCurrentPlayer(_owner_w->_playerInfo._playerId) && _unitData->GetIsUnderwater()) {
                SOUND->PlaySystemSound(SOUND_TYPE_SUBMARINE_DETECTED);
            }
        }
    }
}

bool GameUnit::IsDetectedByPlayer(unsigned int playerId)
{
    bool result = false;
    if (playerId < MAX_PLAYERS)
    {
        result = _config_w->_isStealthable && _unitData->_detected[playerId];
    }
    return result;
}

#pragma mark - Fire methods

bool GameUnit::IsInProcess() const
{
    return _unitData->_isInProcess;
}

bool GameUnit::CanFire(const cocos2d::CCPoint &target)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if (!_unitData->GetConfig()->_isAbleToFire)
        return false;
    if (_unitData->_landed) 
        return false;
    
    if (_unitObject->GetFireing())
        return false;
    
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    return (IsInFireRadius(targetCenter) && _unitData->GetShotBalance() > 0);
}

GameEffect* GameUnit::MakeWeaponAnimationEffect(const cocos2d::CCPoint &target)
{
    int level = _unitData->GetConfig()->_bLevel + 1; // get level from target
    if (level > OBJECT_LEVEL_OVERAIR)
        level = OBJECT_LEVEL_OVERAIR;
    
    MAXUnitObject* _unitObject = GetUnitObject();
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(_unitObject->IsSingleFire()?(_unitObject->params_w->_isInfantry?0.4:0.15):0.5, _unitObject);
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(fireAnim);
    
    BULLET_TYPE type = BULLET_TYPE_NONE;
    SECONDARY_TYPE st = SECONDARY_TYPE_NONE;
    int fireType = _unitData->GetConfig()->_pBulletType;
    if (fireType == 1 || fireType == 4) {
        type = BULLET_TYPE_NONE;
    }
    if (fireType == 3  || fireType == 8) {
        type = BULLET_TYPE_ROCKET;
        st = SECONDARY_TYPE_SMOKE;
    }
    if (fireType == 5) {
        type = BULLET_TYPE_TORPEDO;
        st = SECONDARY_TYPE_RIBBLES;
    }
    if (fireType == 6) {
        type = BULLET_TYPE_PLASMA;
    }
    if (type != BULLET_TYPE_NONE)
    {
        GameEffect* effect = GameEffect::CreateBullet(type, level, BLAST_TYPE_DAMAGEEFFECT, st);
        effect->SetLocation(GetUnitCell());
        effect->Show();
        float coeff = 0.75;
        if (type != BULLET_TYPE_PLASMA) {
            coeff = 1.0;
            effect->SetDirection(MAXObject::CalculateRocketImageIndex(_unitCell, target));
        }
        MAXAnimationObject* anim = new MAXAnimationObject(GetUnitCell(), target, effect->GetObject(), coeff);
        anim->_delegate = effect;
        MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(anim);
        return effect;
    }
    else
    {
        GameEffect* blast = GameEffect::CreateBlast(BLAST_TYPE_DAMAGEEFFECT, level);
        blast->SetLocation(target);
        blast->Show();
        MAXAnimationWait* wait = new MAXAnimationWait(blast->GetFrameCount() * 0.1);
        wait->_delegate = blast;
        MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
        return blast;
    }
}

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if (!CanFire(target))
        return;
    
    _unitData->MakeShot();
    
    MAXUnitObject* _unitObject = GetUnitObject();
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    fireTarget = targetCenter;
    if (_unitObject->params_w->_hasHead)
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));

    _unitData->_headDirection = _unitObject->pureheadIndex;
    _unitData->_bodyDirection = _unitObject->purebodyIndex;
    
    if (selectedGameObjectDelegate)
        selectedGameObjectDelegate->onUnitFireStart(this);
    
    PlaySound(UNIT_SOUND_SHOT);

    GameEffect* effect = MakeWeaponAnimationEffect(targetCenter);
    if (effect)
    {
        effect->_delegate_w = this;
        effect->_tag = GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER;
    }
    else
    {
        PlaySound(UNIT_SOUND_BLAST);
        if (selectedGameObjectDelegate)
            selectedGameObjectDelegate->onUnitFireStop(this);
    }
    
    MAXObjectConfig* config = _unitData->GetConfig();
    if (config->_isStealthable)
    {
        bool processed = false;
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!_unitData->_detected[i])
            {
                _unitData->_detected[i] = true;
                if (!processed)
                {
                    processed = true;
                    GetUnitObject()->StealthDeactivated();
                    _delegate_w->GameUnitDidDetected(this);
                }
            }
        }
    }
}

#pragma mark - Build methods

void GameUnit::StartConstructingUnit(const string &type)
{
    MAXObjectConfig* newUnitConfig = MAXConfigManager::SharedMAXConfigManager()->GetUnitConfig(type);
    if (_unitData->GetIsBuilding())
    {
        
    }
    else
    {
        if (newUnitConfig->_bSize == 1)
        {
            StartConstructingUnitInPlace(GetUnitCell(), type);
        }
        else
        {
            //force MAXGAME to select suitable place
            StartConstructingUnitInPlace(GetUnitCell(), type);
        }
    }
}

void GameUnit::StartConstructingUnitInPlace(const CCPoint &topLeftCell, const string &type)
{
    MAXObjectConfig* newUnitConfig = MAXConfigManager::SharedMAXConfigManager()->GetUnitConfig(type);
    if (newUnitConfig->_bSize == 1)
    {
        _effectUnder = GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE_SMALL, GetObject()->_currentLevel - 2);
        _effectUnder->SetLocation(topLeftCell);
        _effectUnder->Show();
        _effectUnderBuildingTape = GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE_PROGRESS_SMALL, GetObject()->_currentLevel - 1);
        _effectUnderBuildingTape->SetLocation(topLeftCell);
        _effectUnderBuildingTape->Show();
    }
    else
    {
        _effectUnder = GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE_LARGE, GetObject()->_currentLevel - 2);
        _effectUnder->SetLocation(topLeftCell);
        _effectUnder->Show();
        _effectUnderBuildingTape = GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE_PROGRESS_LARGE, GetObject()->_currentLevel - 1);
        _effectUnderBuildingTape->SetLocation(topLeftCell);
        _effectUnderBuildingTape->Show();
    }
    StartBuildProcess();
}

void GameUnit::PauseConstructingUnit()
{
    //if it constructs building - we cant to pause
}

void GameUnit::CancelConstructingUnit()
{}

void GameUnit::EscapeConstructedUnit(const CCPoint &cell)
{
    //if it constructs unit - escape new unit
    //if it constructs building - escape self
}

bool GameUnit::CanStartBuildProcess()
{
	MAXObjectConfig* config = _unitData->GetConfig();
    return ((config->_isAllwaysOn != _unitData->GetIsBuilding()) || config->_bSelfCreatorType != 0 || _unitData->GetIsBuldozer());
}

void GameUnit::StartBuildProcess()
{
    if (!CanStartBuildProcess())
        return;
    
    
    _unitData->_isInProcess = !IsInProcess();
    
    MAXObjectConfig* config = _unitData->GetConfig();
    StopWorkSound();
    if (IsInProcess())
    {
        if (config->_isBuilding && config->_retEnergy == 0)
        {
            workSound = PlaySound(UNIT_SOUND_BUILD);
        }
        else
        {
            workSound = PlaySound(UNIT_SOUND_START);
        }
    }
    else
    {
        workSound = PlaySound(UNIT_SOUND_STOP);
    }
    
    CheckBodyAndShadow();
    ChackForAnimanteBody();
}

vector<UNIT_MENU_ACTION> GameUnit::GetActionList() const
{
	MAXObjectConfig* config = _unitData->GetConfig();
    vector<UNIT_MENU_ACTION> result;
	result.push_back(UNIT_MENU_ACTION_INFO);
	if (movePath.size() > 0)
	{
		result.push_back(UNIT_MENU_ACTION_DONE);
		result.push_back(UNIT_MENU_ACTION_STOP);
	}
	if (config->_isAbleToFire && (_unitData->_pShots > 0))
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
	if ((config->_isAllwaysOn != config->_isBuilding ) || config->_bSelfCreatorType != 0 || config->_isBuldozer)
	{
		if (IsInProcess())
		{
			result.push_back(UNIT_MENU_ACTION_STOP);
		}
		else
		{
			result.push_back(UNIT_MENU_ACTION_START);
		}
	}

    if (config->_bSelfCreatorType != 0)
	{
        result.push_back(UNIT_MENU_ACTION_BUILD);
	}
	if (config->_isInfiltrator && (_unitData->_pShots > 0))
	{
		result.push_back(UNIT_MENU_ACTION_DISABLE);
		result.push_back(UNIT_MENU_ACTION_STEAL);
	}

	if (config->_isBuilding)
	{
		result.push_back(UNIT_MENU_ACTION_REMOVE);
	}
    return result;
}

void GameUnit::CheckMovementUpdate()
{
    CCPoint unitCell = GetUnitCell();
    CCPoint realCell = GetUnitObject()->GetObjectCell();
    realCell.x = (int)(realCell.x + 0.5);
    realCell.y = (int)(realCell.y + 0.5);
    //        printf("unitcell = %f, %f\n", unitCell.x, unitCell.y);
    //        printf("realcell = %f, %f\n\n", realCell.x, realCell.y);
    
    if ((int)unitCell.x != (int)realCell.x || (int)unitCell.y != (int)realCell.y)
    {
        _delegate_w->GameUnitWillLeaveCell(this);
        _unitCell = realCell;
        _unitData->_unitCell = realCell;
        _delegate_w->GameUnitDidEnterCell(this);
        CheckBodyAndShadow();
    }
}

MAXObjectConfig* GameUnit::GetBaseConfig()
{
    return _unitData->GetConfig();
}

int GameUnit::GetParameterMaxValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = _unitData->GetMaxParameterValue(parameterType);
    if ((parameterType == UNIT_PARAMETER_TYPE_SPEED) || (parameterType == UNIT_PARAMETER_TYPE_GAS))
    {
        result /= 10;
    }
    return result;
}
 
int GameUnit::GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = _unitData->GetParameterValue(parameterType);
    if ((parameterType == UNIT_PARAMETER_TYPE_SPEED) || (parameterType == UNIT_PARAMETER_TYPE_GAS))
    {
        result /= 10;
    }
    return result;
}

#pragma mark - MAXAnimationDelegate

void GameUnit::OnAnimationStart(MAXAnimationBase* animation)
{
    if (animation == _currentTopAnimation)
    {
    }
    else // move
    {
        if (movePath.size() > 0 && pathIndex >= 0)
        {
            PFWaveCell* cell = movePath[pathIndex];
            _unitData->MoveWithCost(cell->cost);
            
            if (movePathIndex == pathIndex)
            {
                int soundId = PlaySound(UNIT_SOUND_ENGINE_STOP);
                if (soundId > 0)
                {
                    StopCurrentSound();
                }
            }
            
            pathIndex--;
            if (selectedGameObjectDelegate)
                selectedGameObjectDelegate->onUnitMoveStepBegin(this);
        }
    }
}

void GameUnit::OnAnimationUpdate(MAXAnimationBase* animation)
{
    CheckMovementUpdate();
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitData->_headDirection = _unitObject->pureheadIndex;
    _unitData->_bodyDirection = _unitObject->purebodyIndex;
}

void GameUnit::OnAnimationFinish(MAXAnimationBase* animation)
{
    CheckMovementUpdate();
    
    if (animation == _currentTopAnimation)
    {
        _currentTopAnimation->_delegate = NULL;
        _currentTopAnimation = NULL;
        if (pathIndex < 0)
        {
            // move completed succesfully
            movePath.clear();
            pathIndex = 0;
            pathIsTemp = true;
			if (selectedGameObjectDelegate)
				selectedGameObjectDelegate->onUnitMoveStop(this);
        }
		else
		{
			if (selectedGameObjectDelegate)
				selectedGameObjectDelegate->onUnitMovePause(this);
		}
        StopCurrentSound();
        if (_unitData->GetConfig()->_isPlane)
        {
            if (!_unitData->_landed)
            {
                currentSound = PlaySound(UNIT_SOUND_ENGINE);
            }
        }
        else
        {
            currentSound = PlaySound(UNIT_SOUND_ENGINE);
        }
        
        //MoveToNextCell();
        
    }
    else // move
    {
        if (selectedGameObjectDelegate)
            selectedGameObjectDelegate->onUnitMoveStepEnd(this);
    }
}

#pragma mark - MAXUnitObjectDelegate

int GameUnit::GetScan() const
{
    return _unitData->_unitParameters->_pMaxScan;
}

int GameUnit::GetRange() const
{
    return _unitData->_unitParameters->_pMaxRange;
}

float GameUnit::GetHealStatus() const
{
    return 1.0;
}

float GameUnit::GetShots() const
{
    return _unitData->_pShots;
}

bool GameUnit::ShouldAnimateBody() const
{
    return _shouldAnimateBody;
}

bool GameUnit::ShoudDrawFakeCircle() const
{
	return (movePath.size() > 0) && (!GetIsFreezed() && (game->_match->_currentPlayer_w == _owner_w));
}

CCPoint GameUnit::GetFakeCenter() const
{
	PFWaveCell* cell = movePath[0];
	return CCPointMake(cell->x * 64 + 32, cell->y * 64 + 32);
}

#pragma mark - GameEffectDelegate

void GameUnit::GameEffectDidFinishExistance(GameEffect* effect)
{
    if (effect->_tag == GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER)
    {
        PlaySound(UNIT_SOUND_BLAST);
        if (selectedGameObjectDelegate)
            selectedGameObjectDelegate->onUnitFireStop(this);
    }
}
