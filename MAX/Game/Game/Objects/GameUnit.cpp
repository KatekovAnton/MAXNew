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

using namespace cocos2d;

GameUnit::GameUnit(shared_ptr<MAXUnitObject> unitObject):_unitObject(unitObject)
{
}

GameUnit::~GameUnit()
{}

void GameUnit::SetUnitLocation(const CCPoint& destination, bool animated)
{
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
        if (neededBodyIndex != _unitObject->GetBodyIndex())
        {
            MAXAnimationObjectUnit* step = new MAXAnimationObjectUnit(neededBodyIndex, _unitObject->GetHeadIndex(), _unitObject);
            sequence->AddAnimation(step);
        }
        //this->SetBodyDirection(MAXUnitObject::CalculateImageIndex(currentPosition, destination));
    }
}

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if(_unitObject->GetFireing())
        return;
    _unitObject->SetHeadDirection(MAXUnitObject::CalculateImageIndex(_unitCell, target));
    _unitObject->SetIsFireing(true);
}