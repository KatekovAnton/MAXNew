//
//  MAXAnimationObjectUnit.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationObjectUnit.h"

const float fireTime = 0.15;
const float rotateRime = 0.1;

MAXAnimationObjectUnit::MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, const shared_ptr<MAXUnitObject>& object)      //creates move action
:_unit(object), _startLocation(startLocation), _endLocation(endLocation)
{}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(int bodyIndex, int headIndex, const shared_ptr<MAXUnitObject>& object)                                  //creates rotate action
:_unit(object), _bodyIndex(bodyIndex), _headIndex(headIndex)
{

}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(double firetime, const shared_ptr<MAXUnitObject>& object)                                               //creates fire action
:_unit(object), _firetime(firetime)
{}

bool MAXAnimationObjectUnit::IsFinished()
{
    return false;
}

void MAXAnimationObjectUnit::Update(double time)
{}

