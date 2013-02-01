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

using namespace cocos2d;

GameUnit::GameUnit(shared_ptr<MAXUnitObject> unitObject):_unitObject(unitObject)
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
        _unitObject->SetBodyDirection(MAXUnitObject::CalculateImageIndex(_unitCell, destination));
        _unitCell = destination;
        GLKMatrix4 rt = MAXUnitObject::MatrixForCell(destination);
        _unitObject->SetGlobalPosition(rt, nullptr, nullptr, false);
    }
    else
    {
        int neededBodyIndex = MAXUnitObject::CalculateImageIndex(_unitCell, destination);
        MAXAnimationSequence* sequence = new MAXAnimationSequence();
        sequence->_delegate = this;
        _unitObject->SetBodyDirection(neededBodyIndex);
        if (neededBodyIndex != _unitObject->GetBodyIndex())
        {
            MAXAnimationObjectUnit* step1 = new MAXAnimationObjectUnit(neededBodyIndex, _unitObject->GetHeadIndex(), _unitObject);
            sequence->AddAnimation(step1);
        }
        MAXAnimationObjectUnit* step2 = new MAXAnimationObjectUnit(_unitCell ,destination, _unitObject);
        step2->_delegate = this;
        sequence->AddAnimation(step2);
    
        MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
        _currentTopAnimation = sequence;
    }
}

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if(_unitObject->GetFireing())
        return;
    _unitObject->SetHeadDirection(MAXUnitObject::CalculateImageIndex(_unitCell, target));
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(0.15, _unitObject);
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
