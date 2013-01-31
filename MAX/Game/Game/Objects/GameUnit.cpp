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

using namespace cocos2d;

GameUnit::GameUnit(shared_ptr<MAXUnitObject> unitObject):_unitObject(unitObject)
{
}

GameUnit::~GameUnit()
{}

void GameUnit::SetUnitLocation(const CCPoint& cell, bool animated)
{
    if(!animated)
    {
        _unitObject->SetBodyDirection(MAXUnitObject::CalculateImageIndex(_unitCell, cell));
        _unitCell = cell;
        GLKMatrix4 rt = MAXUnitObject::MatrixForCell(cell);
        _unitObject->SetGlobalPosition(rt, nullptr, nullptr, false);
    }
    else
    {
        _unitObject->SetBodyDirection(MAXUnitObject::CalculateImageIndex(_unitCell, cell));
    }
}

void GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if(_unitObject->GetFireing())
        return;
    _unitObject->SetHeadDirection(MAXUnitObject::CalculateImageIndex(_unitCell, target));
    _unitObject->SetIsFireing(true);
}