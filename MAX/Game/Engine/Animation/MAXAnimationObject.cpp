//
//  MAXAnimationObject.cpp
//  MAX
//
//  Created by  Developer on 10.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationObject.h"
#include "MAXObject.h"
#include "MAXEngine.h"
#include "MAXUnitMaterial.h"

const float moveTime = 0.2;

MAXAnimationObject::MAXAnimationObject(const CCPoint& startLocation, const CCPoint& endLocation, MAXObject* object)
:MAXAnimationBase(), _unit(object), _startLocation(startLocation), _endLocation(endLocation)
{}

bool MAXAnimationObject::IsFinished()
{
    return MAXAnimationBase::GetStartTime() + moveTime <= engine->FullTime();
}

void MAXAnimationObject::Update(double time)
{
    double elapsed = (engine->FullTime()-GetStartTime());
    float fromminonetoone = (2.0*elapsed/moveTime) - 1.0;
    float deltaTime = sinf(fromminonetoone * M_PI_2) * 0.5 + 0.5;
    CCPoint delta = CCPoint(_endLocation.x - _startLocation.x, _endLocation.y - _startLocation.y);
    CCPoint result = CCPoint(_startLocation.x + delta.x * deltaTime, _startLocation.y + delta.y * deltaTime);
    _unit->SetPosition(result);
}

void MAXAnimationObject::CompletlyFinish()
{
    GLKMatrix4 rt = MAXObject::MatrixForCell(_endLocation);
    _unit->SetGlobalPosition(rt, nullptr, nullptr, false);
}

void MAXAnimationObject::StartAnimation()
{}
