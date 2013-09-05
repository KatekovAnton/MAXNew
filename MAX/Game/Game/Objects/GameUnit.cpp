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
#include "USimpleContainer.h"

#include "SoundEngine.h"

#include "GameUnitData.h"
#include "GameMatchPlayer.h"
#include "GameUnitParameters.h"
#include "GameEffect.h"
#include "GameMatch.h"
#include "GameMap.h"
#include "PFWaveCell.h"
#include "MatchMapAgregator.h"

#include "GUTask.h"
#include "GUConstructBuildingTask.h"
#include "GUConstructUnitTask.h"
#include "GUClearzoneTask.h"
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

GameUnit::GameUnit(MAXUnitObject* unitObject, GameUnitParameters* params, int playerId, GameMatchPlayer *owner)
	:GameObject(unitObject, params->GetConfig()), _currentTopAnimation(NULL), _unitData(new GameUnitData(params, playerId)), _effectUnder(NULL), _delegate_w(NULL), pathIndex(0), pathIsTemp(true), _effectOver(NULL), _currentlyProcesedConstructor(false), _owner_w(owner), _removeDelayAnim(NULL), _destroyed(false), _constructor(NULL)
{
    Init();
}


GameUnit::GameUnit(MAXUnitObject* unitObject, GameUnitData* unitdata, GameMatchPlayer *owner)           //creates unit from saved data
	:GameObject(unitObject, unitdata->GetConfig()), _unitData(unitdata), _owner_w(owner), _removeDelayAnim(NULL), _currentSound(0)
{
    Init();
}

GameMatch *GameUnit::GetMatch()
{
	return _owner_w->_match_w;
}

void GameUnit::Init()
{
    GetUnitObject()->_delegate_w = this;
    MAXObjectConfig* config = _unitData->GetConfig();
    GetUnitObject()->_needShadow = !config->_isUnderwater;
    _onDraw = false;
    CheckBodyAndShadow();
    
    if(_unitData->GetIsBuilding() && _unitData->GetConfig()->_isNeedUndercover)
        _effectUnder = GameEffect::CreateBuildingBase(_unitData->GetSize() == 2?BUILDING_BASE_TYPE_LARGE:BUILDING_BASE_TYPE_SMALL, GetConfig()->_bLevel - 1);
    
    CheckForAnimanteBody();
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
    if (_effectOver)
    {
        _effectOver->Hide();
        delete _effectOver;
    }
}

void GameUnit::CheckForAnimanteBody()
{
    _shouldAnimateBody = _unitData->GetConfig()->bodyActiveFrame0 != _unitData->GetConfig()->bodyActiveFrame1 && !_unitData->_disabledByInfiltrator;
}

unsigned int GameUnit::PlayUnitSound(UNIT_SOUND unitSound)
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
    float engVol = 0.4;
  
    switch (unitSound)
    {
        case UNIT_SOUND_BLAST:
            soundStr = &(config->_soundBlastName);
            break;
        case UNIT_SOUND_SHOT:
            soundStr = &(config->_soundShotName);
            vol = 0.4;
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
    unsigned int soundId = -1;
    if (soundStr->length() > 2)
    {
        if (*soundStr == "attack1.wav" || *soundStr == "aplan1.wav") 
            vol = 0.15;
        soundId = SOUND->PlayGameSound(*soundStr, delegate, loop, vol);
    }
    return soundId;
}

void GameUnit::StopCurrentSound()
{
    if (_currentSound > 0)
    {
        SOUND->StopGameSound(_currentSound);
        _currentSound = 0;
    }
}

void GameUnit::UnitDidSelect()
{
    StopCurrentSound();
    if (_owner_w->GetIsCurrentPlayer())
        SOUND->PlaySystemSound(_unitData->GetOnSelectSoundType());
    
    _currentSound = PlayUnitSound(_unitData->GetBackgroundSoundType());
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

void GameUnit::SetColor(GLKVector4 color)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->playerColor = color;
}

void GameUnit::Show()
{
    if (!_unitData->_isUnderConstruction)
        GameObject::Show();
    if (_effectUnder)
        _effectUnder->Show();
    if (_effectOver && _owner_w->GetIsCurrentPlayer())
        _effectOver->Show();
    CheckBodyAndShadow();
}

void GameUnit::Hide()
{
    StopCurrentSound();
    
    GameObject::Hide();
	if (gameObjectDelegate)
		gameObjectDelegate->onUnidHided(this);
    if (_effectUnder)
        _effectUnder->Hide();
    if (_effectOver)
        _effectOver->Hide();
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

        StopCurrentSound();
        _currentSound = this->PlayUnitSound(UNIT_SOUND_START);
    }
}

void GameUnit::Landing()
{
    if (!_unitData->_landed)
    {
        _unitData->_landed = true;
        GetUnitObject()->Landing();
        
        StopCurrentSound();
        PlayUnitSound(UNIT_SOUND_STOP);
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
    
    
    _unitData->StartNewTurn();
    if (_unitData->GetIsTaskFinished())
    {
        if (_owner_w->GetIsCurrentPlayer())
            CreateCheckIcon();
        else
            DestroyCheckIcon();
        CheckBuildProcess();
    }
    else
        DestroyCheckIcon();
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
    _unitData->AbortTask();
    Hide();
    // update the fog of war for the current gamer
	if (_delegate_w)
	    _delegate_w->GameUnitDidRemoveFromMap(this);
}

void GameUnit::Destroy()
{
    _destroyed = true;
    _idleDestroy = false;
    _delegate_w->GameUnitDidDestroy(this);
	gameObjectDelegate->onUnitDestroyed(this);
}

void GameUnit::Destroy(bool isIdle)
{
    _idleDestroy = isIdle;
    _delegate_w->GameUnitDidDestroy(this);
	gameObjectDelegate->onUnitDestroyed(this);
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

bool GameUnit::IsDetectedByCurrentPlayerTeam()
{
    GameMatch *m = GetMatch();
    
    for (int i = 0; i < m->_players.size(); i++)
    {
        GameMatchPlayer *player = m->_players[i];
        if (IsDetectedByPlayer(player->GetPlayerId()) && m->PlayerIsEnemyToPlayer(player, _owner_w) && !m->PlayerIsEnemyToPlayer(player, m->_currentPlayer_w))
            return true;
    }
    return false;
}

bool GameUnit::IsDetectedByEnemies()
{
    GameMatch *m = GetMatch();
    
    for (int i = 0; i < m->_players.size(); i++)
    {
        GameMatchPlayer *player = m->_players[i];
        if (IsDetectedByPlayer(player->GetPlayerId()) && m->PlayerIsEnemyToPlayer(player, _owner_w))
            return true;
    }
    return false;
}

void GameUnit::CheckBodyAndShadow()
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if (!(_unitData->GetIsAmphibious() || _unitData->GetIsUnderwater() || _unitData->GetCanStartBuildProcess()) && _unitData->GetIsBuilding())
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
    
    bool constructingNow = (_unitData->ContainsCurrentTask() && !(_unitData->GetIsTaskFinished() && _unitData->GetIsTaskFinished())) || _unitData->_isOn;
    EXTENDED_GROUND_TYPE groundType = _owner_w->_agregator->GroundTypeAtXY(_unitCell.x, _unitCell.y);
    if (_unitData->GetConfig()->_isBuilding)
    {
        _unitObject->SetBodyOffset((constructingNow && (!_unitData->GetConfig()->_isAllwaysOn || !_unitData->GetConfig()->_isActiveBody))?1:0);
    }
    else
    {
        if (groundType == EXTENDED_GROUND_TYPE_WATER)
        {
            _unitObject->_currentLevel = OBJECT_LEVEL_ONGROUND;
            if (_unitData->GetConfig()->_isUnderwater)
            {
                bool onSurface = false;
                if (_owner_w->GetIsCurrentPlayer())
                    onSurface = IsDetectedByEnemies();
                else
                    onSurface = IsDetectedByCurrentPlayerTeam();
				_unitObject->SetBodyOffset(onSurface?8:0);
                _unitObject->_needShadow = false;
                return;
            }
            if (_unitData->GetIsAmphibious())
            {
                _unitObject->SetBodyOffset(constructingNow?24:8);
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
                _unitObject->SetBodyOffset(constructingNow?16:0);
                return;
            }
            if (_unitData->GetIsBuldozer())
            {
                _unitObject->SetBodyOffset(constructingNow?8:0);
                return;
            }
        }
    }
}

void GameUnit::CheckBuildProcess()
{
    MAXObjectConfig* config = _unitData->GetConfig();
    if (!config->_containProcessState)
        return;
    
    
    StopCurrentSound();
    if (_unitData->ContainsCurrentTask() && !(_unitData->GetIsTaskFinished() && _unitData->GetIsTaskFinished()))
    {
        if (config->_isBuilding && config->_retEnergy == 0)
            _currentSound = PlayUnitSound(UNIT_SOUND_BUILD);
        else
            _currentSound = PlayUnitSound(UNIT_SOUND_WORK);
    }
    else
        _currentSound = PlayUnitSound(UNIT_SOUND_STOP);
    
    CheckBodyAndShadow();
    CheckForAnimanteBody();
}

bool GameUnit::CanMove() const
{
    return _unitData->CanMove();
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
        movePath.clear();
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
        result = movePath[pi];
    
    return result;
}

bool GameUnit::IsPathTargetedTo(const int x, const int y)
{
	bool result = FALSE;
	if (movePath.size() > 0)
	{
		PFWaveCell* cell = movePath[0];
		if (cell && (cell->x == x) && (cell->y == y))
			result = TRUE;
	}
	return result;
}

void GameUnit::ConfirmCurrentPath()
{
	if (movePath.size() > 1)
	{
		if (gameObjectDelegate)
			gameObjectDelegate->onUnitMoveStart(this);

        pathIsTemp = false;
        if (_unitData->GetConfig()->_isPlane)
            TakeOff();
		FollowPath();
	}
}

void GameUnit::AbortCurrentPath()
{
    if (_currentTopAnimation)
        _currentTopAnimation->Stop();
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
        else if (speedMult > 9)
            speedMult = 9;
        else if (speedMult < 5)
            speedMult = 5;
        float moveFactor = cell->cost * 10.0 / speedMult; // change to current max speed
		if (moveFactor < 0.1)
			moveFactor = 20;
		move->SetMoveFactor(moveFactor);
        sequence->AddAnimation(move);
        
        if (first)
        {
            StopCurrentSound();
            _currentSound = PlayUnitSound(UNIT_SOUND_ENGINE_START);
        }
        
        pos = destination;
        pi--;
        first = false;
    }
    _currentTopAnimation = sequence;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
}

void GameUnit::RemoveWithDelay(double delay)
{
    if (_removeDelayAnim) {
        throw "void GameUnit::RemoveWithDelay(double delay) but _removeDelayAnim already exists!";
    }
    MAXAnimationWait *anim = new MAXAnimationWait(delay);
    anim->_delegate = this;
    _removeDelayAnim = anim;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(anim);
}

void GameUnit::RunAnimation(MAXAnimationSequence* animation)
{
    if (_currentTopAnimation) {
        throw "void GameUnit::RunAnimation(MAXAnimationBase* animation) but _currentTopAnimation already exists!";
    }
    
    animation->_delegate = this;
    _currentTopAnimation = animation;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(animation);
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


bool GameUnit::ReceiveDamage(GameUnit* unit)
{
    return ReceiveDamage(unit, 1);
}

bool GameUnit::ReceiveDamage(GameUnit* unit, int decrase)
{
    bool result = _unitData->ReceiveDamage(unit->_unitData, decrase);
    return result;
}

#pragma mark - Connectors

void GameUnit::UpdateConnectors()
{
    
    MAXUnitObject *object = GetUnitObject();
    object->RemoveConnectors();
    if (_destroyed) 
        return;
    
    if (_unitData->GetSize() == 1)
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
		if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR4);
        
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR2);
       
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR1);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y+1);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR3);
    }
    else
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR4);
        cell = ccp(GetUnitCell().x-1, GetUnitCell().y+1);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR8);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+2);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR3);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y+2);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR7);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR1);
        
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y-1);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR5);
        
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR2);
        
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y+1);
        if (GetMatch()->GetIsCellValid(cell) && GetMatch()->_fullAgregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR6);
    }
}

vector<CCPoint> GameUnit::GetNerbyCells()
{
    vector<CCPoint> resuplt;
    vector<CCPoint> allPoints;
    if (_unitData->GetSize() == 1)
    {
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y));
        allPoints.push_back(ccp(GetUnitCell().x+1, GetUnitCell().y));
        allPoints.push_back(ccp(GetUnitCell().x, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x, GetUnitCell().y+1));
    }
    else
    {
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y));
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y+1));
        allPoints.push_back(ccp(GetUnitCell().x, GetUnitCell().y+2));
        allPoints.push_back(ccp(GetUnitCell().x+1, GetUnitCell().y+2));
      
        allPoints.push_back(ccp(GetUnitCell().x, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x+1, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x+2, GetUnitCell().y));
        allPoints.push_back(ccp(GetUnitCell().x+2, GetUnitCell().y+1));
    }
    for (int i = 0; i < allPoints.size(); i++)
    {
        CCPoint cell = allPoints[i];
        if (GetMatch()->GetIsCellValid(cell))
            resuplt.push_back(cell);
    }
    return resuplt;
}

vector<CCPoint> GameUnit::GetFullNearbyCells()
{
    vector<CCPoint> resuplt = GetNerbyCells();
    vector<CCPoint> allPoints;
    if (_unitData->GetSize() == 1)
    {
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x+1, GetUnitCell().y+1));
        allPoints.push_back(ccp(GetUnitCell().x+1, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y+1));
    }
    else
    {
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x+2, GetUnitCell().y-1));
        allPoints.push_back(ccp(GetUnitCell().x-1, GetUnitCell().y+2));
        allPoints.push_back(ccp(GetUnitCell().x+2, GetUnitCell().y+2));
    }
    for (int i = 0; i < allPoints.size(); i++)
    {
        CCPoint cell = allPoints[i];
        if (GetMatch()->GetIsCellValid(cell))
            resuplt.push_back(cell);
    }
    return resuplt;
}

vector<CCPoint> GameUnit::GetUnitCells()
{
    vector<CCPoint> allPoints;
    CCPoint p = GetUnitCell();
    allPoints.push_back(p);
    if (_unitData->GetSize() != 1)
    {
        allPoints.push_back(ccp(p.x + 1, p.y));
        allPoints.push_back(ccp(p.x + 1, p.y + 1));
        allPoints.push_back(ccp(p.x, p.y + 1));
    }
    return allPoints;
}

#pragma mark - Stealth methods

void GameUnit::DetectedByPlayer(unsigned int playerId)
{
    if (!_unitData->_detected[playerId])
    {
        _unitData->_detected[playerId] = true;
        _unitData->_detected[_owner_w->GetPlayerId()] = true;
        if (GetMatch()->GetIsCurrentPlayer(playerId) &&
            !GetMatch()->GetIsCurrentPlayer(_owner_w->GetPlayerId()))
            GetUnitObject()->StealthDeactivated();
        
        if (_owner_w->GetIsCurrentPlayer() && _unitData->GetIsUnderwater() && !_unitData->GetIsAmphibious() && _owner_w->GetPlayerId() != playerId)
            SOUND->PlaySystemSound(SOUND_TYPE_SUBMARINE_DETECTED);
    }
}

void GameUnit::UndetectedByPlayer(unsigned int playerId)
{
    if (_unitData->_detected[playerId])
    {
        _unitData->_detected[playerId] = false;
        _unitData->_detected[_owner_w->GetPlayerId()] = false;//todo:change to  IsDetectedByCurrentPlayerTeam and IsDetectedByEnemies
        if (GetMatch()->GetIsCurrentPlayer(playerId) ||
            GetMatch()->GetIsCurrentPlayer(_owner_w->GetPlayerId()))
            GetUnitObject()->StealthActivated();
        
    }
}

bool GameUnit::IsDetectedByPlayer(unsigned int playerId)
{
    return _unitData->IsDetectedByPlayer(playerId);
}

#pragma mark - Fire methods

bool GameUnit::CanFire(const cocos2d::CCPoint &target)
{
    if (_unitData->_isUnderConstruction)
        return false;
	if (_unitData->GetUniteractable())
        return false;
    if (_unitData->GetConfig()->_isBombMine) 
        return (int)GetUnitCell().x == (int)target.x && (int)GetUnitCell().y == (int)target.y;
    
    
    MAXUnitObject* _unitObject = GetUnitObject();
    if (!_unitData->GetConfig()->_isAbleToFire)
        return false;
    if (_unitData->_landed) 
        return false;
    
    if (_unitObject->GetFireing())
        return false;
    
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    return (_unitData->IsInFireRadius(targetCenter) && (_unitData->GetShotBalance() > 0));
}

GameEffect* GameUnit::MakeWeaponAnimationEffect(const cocos2d::CCPoint &target, int level)
{
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
        effect->_targetCell = target;
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

void GameUnit::Fire(const cocos2d::CCPoint &target, const int level)
{
    if (!CanFire(target))
    {
        throw "GameUnit::Fire cant fire!";
        return;
    }
    
    _unitData->MakeShot();
    
    MAXUnitObject* _unitObject = GetUnitObject();
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    
    if (_unitObject->params_w->_hasHead)
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));

    _unitData->_headDirection = _unitObject->pureheadIndex;
    _unitData->_bodyDirection = _unitObject->purebodyIndex;
    
    if (gameObjectDelegate)
        gameObjectDelegate->onUnitFireStart(this);
    
    PlayUnitSound(UNIT_SOUND_SHOT);

    GameEffect* effect = MakeWeaponAnimationEffect(targetCenter, level);
    if (effect && !GetConfig()->_isBombMine)
    {
        effect->_delegate_w = this;
        if (effect->GetEffectType() != EFFECT_TYPE_BLAST)
            effect->_tag = GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER;
    }
    
    if (effect->GetEffectType() == EFFECT_TYPE_BLAST) {
        if (gameObjectDelegate)
            gameObjectDelegate->onUnitFireStop(this);
    }

}

#pragma mark - Place/remove mines methods

void GameUnit::SetPlacingMines(bool action)
{
    _unitData->SetPlacingMines(action);
    if (_unitData->GetIsPlacingMines()) 
        PerformMineAction();
}

void GameUnit::SetRemovingMines(bool action)
{
    _unitData->SetRemovingMines(action);
    if (_unitData->GetIsRemovingMines()) 
        PerformMineAction();
}

void GameUnit::PerformMineAction()
{
    bool containMine = _owner_w->_match_w->_fullAgregator->IsBombMineInPosition(_unitCell.x, _unitCell.y, (UNIT_MOVETYPE)GetConfig()->_bMoveType);
    
    EXTENDED_GROUND_TYPE gt = _owner_w->_match_w->_fullAgregator->GroundTypeAtXY(_unitCell.x, _unitCell.y);
    if (gt == EXTENDED_GROUND_TYPE_BRIDGE || gt == EXTENDED_GROUND_TYPE_UNPASSABLE) 
        return;
    
    
    if (_unitData->GetIsPlacingMines() && !containMine)
    {
        string mineName = "landmine";
        if (GetConfig()->_bMoveType == UNIT_MOVETYPE_SEA)
            mineName = "seamine";
        
        GameUnit* _newMine = _owner_w->CreateUnit(_unitCell.x, _unitCell.y, mineName, 0);
        _newMine->PlaceUnitOnMap();
        
        _unitData->SetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL, _unitData->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL) - 1);
        SOUND->PlayExplodeSound(GetConfig()->_bMoveType == UNIT_MOVETYPE_SEA?EXPLODE_SOUND_TYPE_SEAMINE_PLACE:EXPLODE_SOUND_TYPE_LANDMINE_PLACE);
        if (_unitData->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL) == 0)
            SetPlacingMines(false);
        
    }
    
    
    if (_unitData->GetIsRemovingMines() && containMine)
    {
        USimpleContainer<GameUnit*> *units = _owner_w->_match_w->_fullAgregator->UnitsInCell(_unitCell.x, _unitCell.y);
        for (int i = 0; i < units->GetCount(); i++)
        {
            GameUnit *unit = units->objectAtIndex(i);
            if (unit->GetConfig()->_isBombMine)
            {
                unit->Destroy(true);
                _unitData->SetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL, _unitData->GetParameterValue(UNIT_PARAMETER_TYPE_MATERIAL) + 1);
                SOUND->PlayExplodeSound(GetConfig()->_bMoveType == UNIT_MOVETYPE_SEA?EXPLODE_SOUND_TYPE_SEAMINE_REMOVE:EXPLODE_SOUND_TYPE_LANDMINE_REMOVE);
                break;
            }
        }
    }
}

#pragma mark - Build methods

void GameUnit::CreateCheckIcon()
{
    if (_effectOver)
        DestroyCheckIcon();
    
    _effectOver = GameEffect::CreateTaskCompletedChack(GetObject()->_currentLevel + 1);
    _effectOver->SetLocation(GetUnitCell());
    if (_owner_w->GetIsCurrentPlayer())
        _effectOver->Show();
}

void GameUnit::DestroyCheckIcon()
{
    if (!_effectOver)
        return;
    
    _effectOver->Hide();
    delete _effectOver;
    _effectOver = NULL;
}

void GameUnit::CreateSmallBuildingTape()
{
    if (_effectUnder)
        DestroyBuildingTape();
    
    CCPoint p = GetUnitCell();
    EXTENDED_GROUND_TYPE gt = _owner_w->_agregator->GroundTypeAtXY(p.x, p.y);
    _effectUnder = GameEffect::CreateBuildingBase(gt != EXTENDED_GROUND_TYPE_WATER?BUILDING_BASE_TYPE_PROGRESS_SMALL:BUILDING_BASE_TYPE_PROGRESS_SEA_SMALL, GetConfig()->_bLevel);
    _effectUnder->SetLocation(GetUnitCell());
    _effectUnder->Show();
}

void GameUnit::CreateLargeBuildingTape()
{
    if (_effectUnder)
        DestroyBuildingTape();
    
    CCPoint p = GetUnitCell();
    EXTENDED_GROUND_TYPE gt = _owner_w->_agregator->GroundTypeAtXY(p.x, p.y);
    _effectUnder = GameEffect::CreateBuildingBase(gt != EXTENDED_GROUND_TYPE_WATER?BUILDING_BASE_TYPE_PROGRESS_LARGE:BUILDING_BASE_TYPE_PROGRESS_SEA_LARGE, GetConfig()->_bLevel);
    _effectUnder->SetLocation(GetUnitCell());
    _effectUnder->Show();
}

void GameUnit::DestroyBuildingTape()
{
    if (!_effectUnder) 
        return;
    
    _effectUnder->Hide();
    delete _effectUnder;
    _effectUnder = NULL;
}

void GameUnit::StartConstructingUnitInPlace(const CCPoint &topLeftCell, MAXObjectConfig *buildingConfig)
{
    GUTask* task = NULL;
    if (buildingConfig->_isBuilding)
    {
        task = new GUConstructBuildingTask(this, buildingConfig->_type, 0, 1, topLeftCell);
        if (buildingConfig->_bSize == 2)
            SetDirection((GetUnitObject()->purebodyIndex/2) * 2);
    }
    
    _unitData->SetTask(task);
    CheckBuildProcess();
}

void GameUnit::PauseConstructingUnit()
{
    //if it constructs building - we cant to pause
}

void GameUnit::CancelConstructingUnit()
{}

void GameUnit::BeginConstructionSequence(GameUnit *constructor)
{
	_constructor = constructor;
    _unitData->_isUnderConstruction = true;
}

void GameUnit::EndConstructionSequense()
{
	_constructor = NULL;
    _delegate_w->GameUnitDidRemoveFromMap(this);
    _unitData->_isUnderConstruction = false;
    _delegate_w->GameUnitDidPlaceOnMap(this);
    PlaceUnitOnMap();
}

void GameUnit::TurnOn()
{
    _unitData->_isOn = true;
    CheckBodyAndShadow();
    CheckBuildProcess();
}

void GameUnit::TurnOf()
{
    _unitData->_isOn = false;
    CheckBuildProcess();
}

void GameUnit::EscapeConstructedUnit(const CCPoint &cell)
{
    //if it constructs unit - escape new unit
    //if it constructs building - escape self
}

void GameUnit::AbortConstructingUnit()
{
    _unitData->AbortTask();
    DestroyBuildingTape();
	DestroyCheckIcon();
    CheckBodyAndShadow();
    StopCurrentSound();
    _currentSound = PlayUnitSound(UNIT_SOUND_ENGINE);
}

vector<UNIT_MENU_ACTION> GameUnit::GetActionList() const
{
    if (_unitData->_isUnderConstruction)
    {
        vector<UNIT_MENU_ACTION> result;
        return result;
    }
    return _unitData->GetActionList(movePath.size()>0);
}

void GameUnit::CheckMovementUpdate()
{
    CCPoint unitCell = GetUnitCell();
    CCPoint realCell = GetUnitObject()->GetObjectCell();
    realCell.x = (int)(realCell.x + 0.5);
    realCell.y = (int)(realCell.y + 0.5);
    
    if ((int)unitCell.x != (int)realCell.x || (int)unitCell.y != (int)realCell.y)
    {
        _delegate_w->GameUnitWillLeaveCell(this);
        _unitCell = realCell;
        _unitData->_unitCell = realCell;
        _delegate_w->GameUnitDidEnterCell(this);
        CheckBodyAndShadow();
        _unitData->OnCellChanged();
        PerformMineAction();
    }
}

MAXObjectConfig* GameUnit::GetConfig()
{
    return _unitData->GetConfig();
}

int GameUnit::GetParameterMaxValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = _unitData->GetMaxParameterValue(parameterType);
    if ((parameterType == UNIT_PARAMETER_TYPE_SPEED) || (parameterType == UNIT_PARAMETER_TYPE_GAS))
        result /= 10;
    return result;
}
 
int GameUnit::GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = _unitData->GetParameterValue(parameterType);
    if ((parameterType == UNIT_PARAMETER_TYPE_SPEED) || (parameterType == UNIT_PARAMETER_TYPE_GAS))
        result /= 10;
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
                int soundId = PlayUnitSound(UNIT_SOUND_ENGINE_STOP);
                if (soundId > 0)
                    StopCurrentSound();
            }
            
            pathIndex--;
            if (gameObjectDelegate)
                gameObjectDelegate->onUnitMoveStepBegin(this);
        }
    }
}

void GameUnit::OnAnimationUpdate(MAXAnimationBase* animation)
{
    if (animation == _removeDelayAnim) 
        return;
    if (animation->UpdatesPosition())
        CheckMovementUpdate();
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitData->_headDirection = _unitObject->pureheadIndex;
    _unitData->_bodyDirection = _unitObject->purebodyIndex;
}

void GameUnit::OnAnimationFinish(MAXAnimationBase* animation)
{
    if (!GetConfig()->_isBuilding)
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
			if (gameObjectDelegate)
				gameObjectDelegate->onUnitMoveStop(this);
        }
		else
		{
			if (gameObjectDelegate)
				gameObjectDelegate->onUnitMovePause(this);
		}
        StopCurrentSound();
        if (_unitData->GetConfig()->_isPlane)
        {
            if (!_unitData->_landed)
                _currentSound = PlayUnitSound(UNIT_SOUND_ENGINE);
        }
        else
            _currentSound = PlayUnitSound(UNIT_SOUND_ENGINE);
    }
    else if (animation == _removeDelayAnim)
    {
        StopCurrentSound();
        RemoveUnitFromMap();
        _owner_w->_units.removeObject(this);
        delete this;
    }
    else // move
    {
        if (gameObjectDelegate)
            gameObjectDelegate->onUnitMoveStepEnd(this);
    }
}

#pragma mark - MAXUnitObjectDelegate

bool GameUnit::ShouldSkipThisUnit() const
{
    return _unitData->_isUnderConstruction;
}

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

bool GameUnit::ShoudDrawFakeCircle()
{
	return (movePath.size() > 0) && (!GetIsFreezed() && (GetMatch()->_currentPlayer_w == _owner_w)) && gameObjectDelegate->IsUnitSelected(this);;
}

CCPoint GameUnit::GetFakeCenter() const
{
	PFWaveCell* cell = movePath[0];
	return ccp(cell->x * 64 + 32, cell->y * 64 + 32);
}

#pragma mark - GameEffectDelegate

void GameUnit::GameEffectDidStartExistance(GameEffect* effect)
{
    if (effect->_tag == GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER)
    {
       
    }
}

void GameUnit::GameEffectDidFinishExistance(GameEffect* effect)
{
    if (effect->_tag == GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER)
    {
        if (gameObjectDelegate)
            gameObjectDelegate->onUnitFireStop(this);
    }
}
