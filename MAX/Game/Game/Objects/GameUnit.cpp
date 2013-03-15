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

#include "GameUnitParameters.h"
#include "GameMatchPlayer.h"
#include "GameEffect.h"
#include "GameMatch.h"
#include "GameMap.h"

using namespace cocos2d;

GameUnit::GameUnit(MAXUnitObject* unitObject, GameUnitParameters* config, GameMatchPlayer* owner)
:GameObject(unitObject, config->GetConfig()), _currentTopAnimation(NULL), _config(config), _owner_w(owner), _effectUnder(NULL), _isInProcess(false)
{
    unitObject->_playerId = owner->_playerInfo._playerId;
    unitObject->_playerPalette_w = &owner->_palette;
    unitObject->_statusDelegate_w = this;
    unitObject->_needShadow = !_config->GetConfig()->_isUnderwater;
    _onMap = false;
    _detected = false;
    if(_config->GetConfig()->_isBuilding && _config->GetConfig()->_isNeedUndercover)
    {
        _effectUnder = GameEffect::CreateBuildingBase(_config->GetConfig()->_bSize == 2?BUILDING_BASE_TYPE_LARGE:BUILDING_BASE_TYPE_SMALL, OBJECT_LEVEL_ONGROUND);
    }
        
}

GameUnit::~GameUnit()
{
    delete _config;
    if (_effectUnder) {
        _effectUnder->RemoveFromMap();
        delete _effectUnder;
    }
}

void GameUnit::SetDirection(int dir)
{
    if(_config->GetConfig()->_isBuilding)
        return;
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->SetBodyDirection(dir);
    _unitObject->SetHeadDirection(dir);
}

void GameUnit::SetRandomDirection()
{
    if(_config->GetConfig()->_isBuilding)
        return;
    SetDirection(nextIntMax(8));
}

void GameUnit::LocateOnMap()
{
    GameObject::LocateOnMap();
    if (_effectUnder)
        _effectUnder->LocateOnMap();
}

void GameUnit::RemoveFromMap()
{
    GameObject::RemoveFromMap();
    if (_effectUnder)
        _effectUnder->RemoveFromMap();
}

void GameUnit::SetLocation(const cocos2d::CCPoint &cell)
{
    GameObject::SetLocation(cell);
    if (_effectUnder) 
        _effectUnder->SetLocation(_unitCell);
}

void GameUnit::CheckBodyAndShadow()
{
    if (!(_config->GetConfig()->_isAmphibious || _config->GetConfig()->_isUnderwater || _config->GetConfig()->_bSelfCreatorType != 0))
        return;
    
    
    MAXUnitObject* _unitObject = GetUnitObject();
    char groundType = _owner_w->_match_w->_map->GroundTypeAtPoint(_unitCell);
    if (groundType == GROUND_TYPE_WATER)
    {
        if (_config->GetConfig()->_isUnderwater && !_detected)
        {
            _unitObject->SetBodyOffset(0);
            _unitObject->_needShadow = false;
            return;
        }
        if (_config->GetConfig()->_isAmphibious)
        {
            _unitObject->SetBodyOffset(_isInProcess?24:8);
            return;
        }
    }
    else
    {
        if (_config->GetConfig()->_isUnderwater)
        {
            _unitObject->SetBodyOffset(8);
            _unitObject->_needShadow = _config->GetConfig()->_haveShadow;
            return;
        }
        if (_config->GetConfig()->_isAmphibious)
        {
            _unitObject->SetBodyOffset(_isInProcess?16:0);
            return;
        }
    }
    
}

void GameUnit::LowerPlane()
{}

void GameUnit::LiftPlane()
{}

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
        MAXAnimationObjectUnit* step1 = new MAXAnimationObjectUnit(neededBodyIndex, _unitObject->GetPureHeadIndex(), _unitObject);
        sequence->AddAnimation(step1);
    }
    MAXAnimationObjectUnit* step2 = new MAXAnimationObjectUnit(_unitCell ,destination, _unitObject);
    step2->_delegate = this;
    sequence->AddAnimation(step2);
    _moveAnimation = step2;
    
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
    _currentTopAnimation = sequence;
}

#pragma mark - Fire methods

bool GameUnit::CanFire(const cocos2d::CCPoint &target)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if(!_config->GetConfig()->_isAbleToFire)
        return false;
    if(_unitObject->GetFireing())
        return false;
    
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    float distance = sqrtf((targetCenter.x - _unitCell.x)*(targetCenter.x - _unitCell.x) + (targetCenter.y - _unitCell.y)*(targetCenter.y - _unitCell.y));
    return (float)_config->_pRange + 0.5f >= distance && _config->_pAmmo != 0 && _config->_pShots != 0;
}

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if (!CanFire(target)) 
        return;
    MAXUnitObject* _unitObject = GetUnitObject();
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    if (_unitObject->params_w->_hasHead)
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(_unitObject->IsSingleFire()?0.15:0.3, _unitObject);
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(fireAnim);
    
    BULLET_TYPE type = BULLET_TYPE_ROCKET;
    GameEffect* effect = GameEffect::CreateBullet(type, _config->GetConfig()->_bLevel, BLAST_TYPE_AIR, SECONDARY_TYPE_SMOKE);
    effect->SetLocation(GetUnitCell());
    effect->LocateOnMap();
    if (type != BULLET_TYPE_PLASMA) {
        effect->SetDirection(MAXObject::CalculateRocketImageIndex(_unitCell, targetCenter));
    }
    
    MAXAnimationObject* anim = new MAXAnimationObject(GetUnitCell(), targetCenter, effect->GetObject());
    anim->_delegate = effect;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(anim);
}

bool GameUnit::CanStartBuildProcess()
{
    return _config->GetConfig()->_bSelfCreatorType != 0 && !_isInProcess;
}

void GameUnit::StartBuildProcess()
{
    if (!CanStartBuildProcess())
        return;
    _isInProcess = true;
    MAXUnitObject* object = (MAXUnitObject*)GetObject();
    if (_config->GetConfig()->_isBuilding)
    {
        object->SetBodyOffset(1);
    }
    else
    {
        object->SetBodyOffset(16);
    }
}

#pragma mark - MAXAnimationDelegate

void GameUnit::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameUnit::OnAnimationUpdate(MAXAnimationBase* animation)
{
    if (animation == _moveAnimation)
    {
    }
}

void GameUnit::OnAnimationFinish(MAXAnimationBase* animation)
{
    if (animation == _moveAnimation)
    {
        _unitCell = ((MAXAnimationObjectUnit*)animation)->GetEendLocation();
        _moveAnimation->_delegate = NULL;
        _moveAnimation = NULL;
        CheckBodyAndShadow();
    }
    else
    {
        _currentTopAnimation->_delegate = NULL;
        _currentTopAnimation = NULL;
    }
}

#pragma mark - MAXStatusRendererUnitDelegate

int GameUnit::GetScan() const
{
    return _config->_pScan;
}

int GameUnit::GetRange() const
{
    return _config->_pRange;
}

float GameUnit::GetHealStatus() const
{
    return 1.0;
}

float GameUnit::GetShots() const
{
    return _config->_pShots;
}

