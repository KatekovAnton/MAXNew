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

#include "SoundEngine.h"

#include "GameUnitCurrentState.h"
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
:GameObject(unitObject, params->GetConfig()), _currentTopAnimation(NULL), _unitCurrentParameters(new GameUnitCurrentState(params)), _effectUnder(NULL), _isInProcess(false), _isPlacedOnMap(false), _delegate_w(NULL), _disabledByInfiltrator(false), pathIndex(0), pathIsTemp(true)
{
    unitObject->_delegate_w = this;
    MAXObjectConfig* config = _unitCurrentParameters->_unitBaseParameters->GetConfig();
    unitObject->_needShadow = !config->_isUnderwater;
    _onDraw = false;
    if (config->_isBuilding && config->_isAllwaysOn)
    {
        _isInProcess = true;
        CheckBodyAndShadow();
    }
    
    currentSound = -1;
    
    _isStealthable = config->_isStealthable;
    
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isNeedUndercover)
    {
        _effectUnder = GameEffect::CreateBuildingBase(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 2?BUILDING_BASE_TYPE_LARGE:BUILDING_BASE_TYPE_SMALL, OBJECT_LEVEL_ONGROUND);
    }
    ChackForAnimanteBody();
}

GameUnit::~GameUnit()
{
    delete _unitCurrentParameters;

    if (_effectUnder) {
        _effectUnder->Hide();
        delete _effectUnder;
    }
}

void GameUnit::ChackForAnimanteBody()
{
    _shouldAnimateBody = _unitCurrentParameters->_unitBaseParameters->GetConfig()->bodyActiveFrame0 != _unitCurrentParameters->_unitBaseParameters->GetConfig()->bodyActiveFrame1 && !_disabledByInfiltrator;//(_isInProcess && _unitCurrentParameters->_unitBaseParameters->GetIsBuilding() && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn) && !_disabledByInfiltrator && !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuldozer && !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_hasHead;
}

int GameUnit::PlaySound(UNIT_SOUND unitSound)
{
    MAXObjectConfig* config = _unitCurrentParameters->_unitBaseParameters->GetConfig();
    string* soundStr = NULL;
    bool loop = false;
    SoundEngineDelegate* delegate = NULL;
    
    switch (unitSound)
    {
        case UNIT_SOUND_BLAST:
            soundStr = &(config->_soundBlastName);
            break;
        case UNIT_SOUND_SHOT:
            soundStr = &(config->_soundShotName);
            break;
        case UNIT_SOUND_ENGINE:
            soundStr = &(config->_soundEngineName);
            loop = true;
            break;
        case UNIT_SOUND_ENGINE_START:
            soundStr = &(config->_soundEngineStartName);
            break;
        case UNIT_SOUND_ENGINE_STOP:
            soundStr = &(config->_soundEngineStopName);
            break;
        case UNIT_SOUND_ENGINE_START_WATER:
            soundStr = &(config->_soundEngineStartWaterName);
            break;
        case UNIT_SOUND_ENGINE_STOP_WATER:
            soundStr = &(config->_soundEngineStopWaterName);
            break;
    }
    int soundId = -1;
    if (soundStr->length() > 2)
    {
        soundId = SOUND->PlayGameSound(*soundStr, delegate, loop);
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

void GameUnit::UnitDidSelect()
{
    StopCurrentSound();
    SOUND->PlaySystemSound(SOUND_TYPE_READY); // SOUND_TYPE_UNIT_READY
    currentSound = PlaySound(UNIT_SOUND_ENGINE);
}

void GameUnit::UnitDidDeselect()
{
    StopCurrentSound();
}

void GameUnit::SetDirection(int dir)
{
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
        return;
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->SetBodyDirection(dir);
    _unitObject->SetHeadDirection(dir);
}

void GameUnit::SetRandomDirection()
{
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
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
    if (_unitCurrentParameters->_landed)
    {
        _unitCurrentParameters->_landed = false;
        GetUnitObject()->TakeOff();
    }
}

void GameUnit::Landing()
{
    if (!_unitCurrentParameters->_landed)
    {
        _unitCurrentParameters->_landed = true;
        GetUnitObject()->Landing();
    }
}

void GameUnit::LiftBridgeInstantly()
{
    GetUnitObject()->LiftBridgeInstantly();
}

void GameUnit::LandInstantly()
{
    if (!_unitCurrentParameters->_landed)
    {
        _unitCurrentParameters->_landed = true;
        GetUnitObject()->LandInstantly();
    }
}

void GameUnit::EscapeToLocation(const int x, const int y, const int cost)
{
    movePath.clear();
    pathIndex = 0;
    pathIsTemp = true;

    _unitCurrentParameters->MoveWithCost(cost);

    CCPoint destination = CCPointMake(x, y);
    int direction = MAXObject::CalculateImageIndex(_unitCell, destination);
    _delegate_w->GameUnitWillLeaveCell(this);
    SetLocation(destination);
    _delegate_w->GameUnitDidEnterCell(this);
    
    SetDirection(direction);
}

void GameUnit::NewTurn()
{
    bool processed = false;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (_unitCurrentParameters->_detected[i])
        {
            _unitCurrentParameters->_detected[i] = false;
            if (!processed)
            {
                processed = true;
                GetUnitObject()->StealthActivated();
                _delegate_w->GameUnitDidUndetected(this);
            }
        }
    }
    _unitCurrentParameters->StartNewTurn();
}

void GameUnit::PlaceUnitOnMap()
{
    if (_isPlacedOnMap) 
        return;
    _isPlacedOnMap = true;
    Show();
    // update the fog of war for the current gamer
    _delegate_w->GameUnitDidPlaceOnMap(this);
}

void GameUnit::RemoveUnitFromMap()
{
    if (!_isPlacedOnMap)
        return;
    _isPlacedOnMap = false;
    Hide();
    // update the fog of war for the current gamer
    _delegate_w->GameUnitDidRemoveFromMap(this);
}

void GameUnit::SetLocation(const cocos2d::CCPoint &cell)
{
    GameObject::SetLocation(cell);
    movePath.clear();
    pathIndex = 0;
    if (_effectUnder) 
        _effectUnder->SetLocation(_unitCell);
}

void GameUnit::CheckBodyAndShadow()
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if (!(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater || CanStartBuildProcess()) && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        //all passive-worked buildings, which cannot be topped by infiltrator
        if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
        {
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 1)
                _unitObject->SetBodyOffset(0);//radar
            else
                _unitObject->SetBodyOffset(_unitCurrentParameters->_unitBaseParameters->GetConfig()->bodyIdleFrame0);//hangar
        }
        return;
    };
    
    
    EXTENDED_GROUND_TYPE groundType = game->_match->_agregator->GroundTypeAtXY(_unitCell.x, _unitCell.y);
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        _unitObject->SetBodyOffset((_isInProcess && (!_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn || !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isActiveBody))?1:0);
    }
    else
    {
        if (groundType == EXTENDED_GROUND_TYPE_WATER)
        {
            _unitObject->_currentLevel = OBJECT_LEVEL_ONGROUND;
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater)
            {
                _unitObject->SetBodyOffset(IsDetectedByPlayer(game->_match->_currentPlayer_w->_playerInfo._playerId)?8:0);
                _unitObject->_needShadow = false;
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious)
            {
                _unitObject->SetBodyOffset(_isInProcess?24:8);
                return;
            }
        }
        else
        {
            _unitObject->_currentLevel = _unitObject->params_w->_bLevel;
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater)
            {
                _unitObject->SetBodyOffset(8);
                _unitObject->_needShadow = _unitCurrentParameters->_unitBaseParameters->GetConfig()->_haveShadow;
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious)
            {
                _unitObject->SetBodyOffset(_isInProcess?16:0);
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuldozer)
            {
                _unitObject->SetBodyOffset(_isInProcess?8:0);
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
        if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isPlane)
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
    MAXObjectConfig* config = _unitCurrentParameters->_unitBaseParameters->GetConfig();

    MAXUnitObject* _unitObject = GetUnitObject();
    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    sequence->_delegate = this;
    CCPoint pos = _unitCell;
    int bodyIndex = _unitObject->GetBodyIndex();
    int pi = pathIndex;
    bool first = true;
    int speed = _unitCurrentParameters->GetMoveBalance();
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
				float moveFactor = cell2->cost * 10.0 / config->_pSpeed; // change to current max speed
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
        float moveFactor = cell->cost * 10.0 / config->_pSpeed; // change to current max speed
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

bool GameUnit::GetIsConnectored()const
{
    return _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isRetranslator || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isConnector;
}

void GameUnit::UpdateConnectors()
{
    MAXUnitObject *object = GetUnitObject();
    object->RemoveConnectors();
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 1)
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
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 1)
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
        if (_isStealthable && !_unitCurrentParameters->_detected[playerId])
        {
            _unitCurrentParameters->_detected[playerId] = true;
            _unitCurrentParameters->_detected[_owner_w->_playerInfo._playerId] = true;
            if (game->_match->GetIsCurrentPlayer(playerId) ||
                game->_match->GetIsCurrentPlayer(_owner_w->_playerInfo._playerId))
            {
                GetUnitObject()->StealthDeactivated();
                _delegate_w->GameUnitDidDetected(this);
            }
            if (game->_match->GetIsCurrentPlayer(_owner_w->_playerInfo._playerId) && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater) {
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
        result = _isStealthable && _unitCurrentParameters->_detected[playerId];
    }
    return result;
}

#pragma mark - Fire methods

bool GameUnit::CanFire(const cocos2d::CCPoint &target)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if(!_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAbleToFire)
        return false;
    if(_unitObject->GetFireing())
        return false;
    
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    return (IsInFireRadius(targetCenter) && _unitCurrentParameters->GetShotBalance() > 0);
}

GameEffect* GameUnit::MakeWeaponAnimationEffect(const cocos2d::CCPoint &target)
{
    int level = _unitCurrentParameters->_unitBaseParameters->GetConfig()->_bLevel + 1; // get level from target
    if (level > OBJECT_LEVEL_OVERAIR)
        level = OBJECT_LEVEL_OVERAIR;
    
    MAXUnitObject* _unitObject = GetUnitObject();
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(_unitObject->IsSingleFire()?(_unitObject->params_w->_isInfantry?0.4:0.15):0.4, _unitObject);
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(fireAnim);
    
    BULLET_TYPE type = BULLET_TYPE_NONE;
    SECONDARY_TYPE st = SECONDARY_TYPE_NONE;
    int fireType = _unitCurrentParameters->_unitBaseParameters->GetConfig()->_pBulletType;
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
    
    _unitCurrentParameters->MakeShot();
    
    MAXUnitObject* _unitObject = GetUnitObject();
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    fireTarget = targetCenter;
    if (_unitObject->params_w->_hasHead)
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));

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
    
    MAXObjectConfig* config = _unitCurrentParameters->_unitBaseParameters->GetConfig();
    if (config->_isStealthable)
    {
        bool processed = false;
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!_unitCurrentParameters->_detected[i])
            {
                _unitCurrentParameters->_detected[i] = true;
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

bool GameUnit::CanStartBuildProcess()
{
	MAXObjectConfig* config = _unitCurrentParameters->_unitBaseParameters->GetConfig();
    return ((config->_isAllwaysOn != config->_isBuilding ) || config->_bSelfCreatorType != 0 || config->_isBuldozer);
}

void GameUnit::StartBuildProcess()
{
    if (!CanStartBuildProcess())
        return;
    _isInProcess = !_isInProcess;
    CheckBodyAndShadow();
    ChackForAnimanteBody();
}

vector<UNIT_MENU_ACTION> GameUnit::GetActionList() const
{
	MAXObjectConfig* config = _unitCurrentParameters->_unitBaseParameters->GetConfig();
    vector<UNIT_MENU_ACTION> result;
	result.push_back(UNIT_MENU_ACTION_INFO);
	if (movePath.size() > 0)
	{
		result.push_back(UNIT_MENU_ACTION_DONE);
		result.push_back(UNIT_MENU_ACTION_STOP);
	}
	if (config->_isAbleToFire && (_unitCurrentParameters->_pShots > 0))
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
		result.push_back(UNIT_MENU_ACTION_UPGRADE);
	}	
	if ((config->_isAllwaysOn != config->_isBuilding ) || config->_bSelfCreatorType != 0 || config->_isBuldozer)
	{
		if (_isInProcess)
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
	if (config->_isInfiltrator && (_unitCurrentParameters->_pShots > 0))
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
        //            printf("Update radar!\n");
        _delegate_w->GameUnitWillLeaveCell(this);
        _unitCell = realCell;
        _delegate_w->GameUnitDidEnterCell(this);
        CheckBodyAndShadow();
    }
}

MAXObjectConfig* GameUnit::GetBaseConfig()
{
    return _unitCurrentParameters->_unitBaseParameters->GetConfig();
}

int GameUnit::GetParameterMaxValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = _unitCurrentParameters->GetMaxParameterValue(parameterType);
    if ((parameterType == UNIT_PARAMETER_TYPE_SPEED) || (parameterType == UNIT_PARAMETER_TYPE_GAS))
    {
        result /= 10;
    }
    return result;
}
 
int GameUnit::GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = _unitCurrentParameters->GetParameterValue(parameterType);
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
            _unitCurrentParameters->MoveWithCost(cell->cost);
            
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
        currentSound = PlaySound(UNIT_SOUND_ENGINE);
        
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
    return _unitCurrentParameters->_unitBaseParameters->_pMaxScan;
}

int GameUnit::GetRange() const
{
    return _unitCurrentParameters->_unitBaseParameters->_pMaxRange;
}

float GameUnit::GetHealStatus() const
{
    return 1.0;
}

float GameUnit::GetShots() const
{
    return _unitCurrentParameters->_pShots;
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
