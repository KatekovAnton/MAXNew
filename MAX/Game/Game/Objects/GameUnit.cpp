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
:GameObject(unitObject), _currentTopAnimation(NULL), _config(config), _owner_w(owner)
{
    unitObject->_playerId = owner->_playerInfo._playerId;
    unitObject->_playerPalette_w = owner->_palette;
    unitObject->_statusDelegate_w = this;
    unitObject->_needShadow = !_config->GetCongig()->_isUnderwater;
    _onMap = false;
    _detected = false;
}

GameUnit::~GameUnit()
{
    delete _config;
}

void GameUnit::SetDirection(int dir)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->SetBodyDirection(dir);
    _unitObject->SetHeadDirection(dir);
}

void GameUnit::SetRandomDirection()
{
    SetDirection(nextIntMax(8));
}

void GameUnit::CheckBodyAndShadow()
{
    if (!(_config->GetCongig()->_isAmphibious || _config->GetCongig()->_isUnderwater))
        return;
    
    
    MAXUnitObject* _unitObject = GetUnitObject();
    char groundType = _owner_w->_match_w->_map->GroundTypeAtPoint(_unitCell);
    if (groundType == GROUND_TYPE_WATER)
    {
        if (_config->GetCongig()->_isUnderwater && !_detected)
        {
            _unitObject->SetBodyOffset(0);
            _unitObject->_needShadow = false;
            return;
        }
        if (_config->GetCongig()->_isAmphibious)
        {
            _unitObject->SetBodyOffset(8);
            return;
        }
    }
    else
    {
        if (_config->GetCongig()->_isUnderwater)
        {
            _unitObject->SetBodyOffset(8);
            _unitObject->_needShadow = _config->GetCongig()->_haveShadow;
            return;
        }
        if (_config->GetCongig()->_isAmphibious)
        {
            _unitObject->SetBodyOffset(0);
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

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    if(!_config->GetCongig()->_isAbleToFire)
        return;
    if(_unitObject->GetFireing())
        return;
    if (_unitObject->params_w->_hasHead)
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(_unitObject->IsSingleFire()?0.15:0.3, _unitObject);
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(fireAnim);
    
    BULLET_TYPE type = BULLET_TYPE_ROCKET;
    GameEffect* effect = GameEffect::CreateBullet(type, _config->GetCongig()->_bLevel, BLAST_TYPE_BUILDING, SECONDARY_TYPE_SMOKE);
    effect->SetLocation(GetUnitCell());
    effect->LocateOnMap();
    if (type != BULLET_TYPE_PLASMA) {
        effect->SetDirection(MAXObject::CalculateRocketImageIndex(_unitCell, targetCenter));
    }
    
    MAXAnimationObject* anim = new MAXAnimationObject(GetUnitCell(), targetCenter, effect->GetObject());
    anim->_delegate = effect;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(anim);
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
