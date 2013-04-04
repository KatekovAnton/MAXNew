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

#include "GameUnitCurrentState.h"
#include "GameUnitParameters.h"
#include "GameEffect.h"
#include "GameMatch.h"
#include "GameMap.h"
#include "PFWaveCell.h"

using namespace cocos2d;

GameUnit::GameUnit(MAXUnitObject* unitObject, GameUnitParameters* config)
:GameObject(unitObject, config->GetConfig()), _currentTopAnimation(NULL), _unitCurrentParameters(new GameUnitCurrentState(config)), _effectUnder(NULL), _isInProcess(false), _isPlacedOnMap(false), _delegate_w(NULL)
{
    unitObject->_statusDelegate_w = this;
    unitObject->_needShadow = !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater;
    _onMap = false;
    _detected = false;
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isNeedUndercover)
    {
        _effectUnder = GameEffect::CreateBuildingBase(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 2?BUILDING_BASE_TYPE_LARGE:BUILDING_BASE_TYPE_SMALL, OBJECT_LEVEL_ONGROUND);
    }
}

GameUnit::~GameUnit()
{
    delete _unitCurrentParameters;

    if (_effectUnder) {
        _effectUnder->Hide();
        delete _effectUnder;
    }
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
    GameObject::Show();
    if (_effectUnder)
        _effectUnder->Show();
    CheckBodyAndShadow();
}

void GameUnit::Hide()
{
    GameObject::Hide();
    if (_effectUnder)
        _effectUnder->Hide();
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
    if (_effectUnder) 
        _effectUnder->SetLocation(_unitCell);
}

void GameUnit::CheckBodyAndShadow()
{
    if (!(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater || CanStartBuildProcess()))
        return;
    
    
    MAXUnitObject* _unitObject = GetUnitObject();
    GROUND_TYPE groundType = game->_match->_map->GroundTypeAtPoint(_unitCell);
    if (groundType == GROUND_TYPE_WATER)
    {
        if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
        {
            _unitObject->SetBodyOffset(_isInProcess?1:0);
        }
        else
        {
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater && !_detected)
            {
                _unitObject->SetBodyOffset(0);
                _unitObject->_needShadow = false;
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious)
            {
                _unitObject->SetBodyOffset(_isInProcess?24:8);
                return;
            }
        }
    }
    else
    {
        if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
        {
            _unitObject->SetBodyOffset(_isInProcess?1:0);
        }
        else
        {
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
        }
    }
    
}

void GameUnit::LowerPlane()
{}

void GameUnit::LiftPlane()
{}

void GameUnit::SetPath(std::vector<PFWaveCell*> path)
{
    movePath = path;
    pathIndex = movePath.size() - 2; // last value is current position
    
    
    if (selectedGameObjectDelegate)
        selectedGameObjectDelegate->onUnitStartMove(this);
    
    
    FollowPath();
}

void GameUnit::FollowPath(void)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    sequence->_delegate = this;
    CCPoint pos = _unitCell;
    int bodyIndex = _unitObject->GetBodyIndex();
    int pi = pathIndex;
    bool first = true;
    while (pi >= 0)
    {
        PFWaveCell* cell = movePath[pi];
        int pathSize = movePath.size();
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
        MAXANIMATION_CURVE curve;
        if (pathSize == 2)
            curve = MAXANIMATION_CURVE_EASE_IN_OUT;
        else if (pi == pathSize - 2)
            curve = MAXANIMATION_CURVE_EASE_IN;
        else if (pi == 0)
            curve = MAXANIMATION_CURVE_EASE_OUT;
        else
            curve = MAXANIMATION_CURVE_EASE_LINEAR;
        
        MAXAnimationObjectUnit* move = new MAXAnimationObjectUnit(pos ,destination, _unitObject, curve);
        move->_delegate = this;
        float moveFactor = cell->cost * 10.0 / _unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed; // change to current max speed
        move->SetMoveFactor(moveFactor);
        sequence->AddAnimation(move);
        
        pos = destination;
        pi--;
        first = false;
    }
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
    _currentTopAnimation = sequence;
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

#pragma mark - Fire methods

bool GameUnit::CanFire(const cocos2d::CCPoint &target)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if(!_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAbleToFire)
        return false;
    if(_unitObject->GetFireing())
        return false;
    
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    return IsInFireRadius(targetCenter) && _unitCurrentParameters->_unitBaseParameters->_pMaxAmmo != 0 && _unitCurrentParameters->_unitBaseParameters->_pMaxShots != 0;
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
    GameEffect* effect = GameEffect::CreateBullet(type, _unitCurrentParameters->_unitBaseParameters->GetConfig()->_bLevel, BLAST_TYPE_AIR, SECONDARY_TYPE_SMOKE);
    effect->SetLocation(GetUnitCell());
    effect->Show();
    if (type != BULLET_TYPE_PLASMA) {
        effect->SetDirection(MAXObject::CalculateRocketImageIndex(_unitCell, targetCenter));
    }
    
    MAXAnimationObject* anim = new MAXAnimationObject(GetUnitCell(), targetCenter, effect->GetObject());
    anim->_delegate = effect;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(anim);
}

bool GameUnit::CanStartBuildProcess()
{
    return ((!_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding) || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSelfCreatorType != 0);
}

void GameUnit::StartBuildProcess()
{
    if (!CanStartBuildProcess())
        return;
    _isInProcess = !_isInProcess;
    CheckBodyAndShadow();
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

#pragma mark - MAXAnimationDelegate

void GameUnit::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameUnit::OnAnimationUpdate(MAXAnimationBase* animation)
{
    CheckMovementUpdate();
}

void GameUnit::OnAnimationFinish(MAXAnimationBase* animation)
{
//    if (animation == _moveAnimation)
//    {
//        _unitCell = ((MAXAnimationObjectUnit*)animation)->GetEndLocation();
//        _moveAnimation->_delegate = NULL;
//        _moveAnimation = NULL;
////        CheckBodyAndShadow();
//
//        
//        BoundingBox bb;
//        bb.min.x = _unitCell.x;
//        bb.min.y = _unitCell.y;
//        bb.max.x = _unitCell.x + 1;
//        bb.max.y = _unitCell.y + 1;
//        USimpleContainer<MAXObject*> *buffer = new USimpleContainer<MAXObject*>(10);
//        engine->GetAllObjectsInArea(bb, buffer);
//        delete buffer;
//    }
//    else
    CheckMovementUpdate();
    
    if (animation == _currentTopAnimation)
    {
        _currentTopAnimation->_delegate = NULL;
        _currentTopAnimation = NULL;
        if (pathIndex < 0)
        {
            // move completed succesfully
            movePath.clear();
            if (selectedGameObjectDelegate)
                selectedGameObjectDelegate->onUnitStopMove(this);
        }
        //MoveToNextCell();
        
    }
    else // move
    {
        pathIndex--;
    }
}

#pragma mark - MAXStatusRendererUnitDelegate

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

