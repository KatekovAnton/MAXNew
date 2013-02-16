//
//  Unit.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameUnit.h"
#include "Geometry.h"
#include "MAXUnitObject.h"
#include "MAXAnimationPrefix.h"
#include "MAXEngine.h"
#include "MAXUnitConfig.h"

using namespace cocos2d;

GameUnit::GameUnit(shared_ptr<MAXUnitObject> unitObject, MAXUnitConfig* config)
:_unitObject(unitObject), _currentTopAnimation(NULL), _config(config)
{
}

GameUnit::~GameUnit()
{}

void GameUnit::SetUnitLocation(const CCPoint& destination, bool animated)
{
    if (animated && _currentTopAnimation) {
        return;
    }
    
    if(!animated)
    {
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, destination));
        _unitCell = destination;
        _unitObject->SetPosition(destination);
    }
    else
    {
        int neededBodyIndex = MAXObject::CalculateImageIndex(_unitCell, destination);
        MAXAnimationSequence* sequence = new MAXAnimationSequence();
        sequence->_delegate = this;
        //_unitObject->SetBodyDirection(neededBodyIndex);
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
}

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if(!_config->_isAbleToFire)
        return;
    if(_unitObject->GetFireing())
        return;
    if (_unitObject->params._hasHead) 
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(_unitObject->IsSingleFire()?0.15:0.3, _unitObject);
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(fireAnim);
}

#pragma mark - MAXAnimationDelegate

void GameUnit::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameUnit::OnAnimationUpdate(MAXAnimationBase* animation)
{
    //update radar range etc
}

void GameUnit::OnAnimationFinish(MAXAnimationBase* animation)
{
    if (animation == _moveAnimation)
    {
        _unitCell = ((MAXAnimationObjectUnit*)animation)->GetEendLocation();
        _moveAnimation->_delegate = NULL;
        _moveAnimation = NULL;
    }
    else
    {
        _currentTopAnimation->_delegate = NULL;
        _currentTopAnimation = NULL;
    }
}
