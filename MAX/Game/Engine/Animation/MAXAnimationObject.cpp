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



MAXAnimationObject::MAXAnimationObject(const CCPoint& startLocation, const CCPoint& endLocation, MAXObject* object)
:MAXAnimationBase(), _unit(object), _startLocation(startLocation), _endLocation(endLocation)
{
    _aniTime = 0.05 * sqrtf((startLocation.x - endLocation.x) * (startLocation.x - endLocation.x) + (startLocation.y - endLocation.y) * (startLocation.y - endLocation.y));
    _moveCurve = MAXANIMATION_CURVE_EASE_LINEAR;
    _deltaLocation = CCPoint(_endLocation.x - _startLocation.x, _endLocation.y - _startLocation.y);
}

void MAXAnimationObject::Update(double time)
{
    double elapsed = (engine->FullTime()-GetStartTime());
    float deltaTime = GetAniElapsedPart(elapsed);
    CCPoint result = CCPoint(_startLocation.x + _deltaLocation.x * deltaTime, _startLocation.y + _deltaLocation.y * deltaTime);
    _unit->SetPosition(result);
}

void MAXAnimationObject::CompletlyFinish()
{
    GLKMatrix4 rt = MAXObject::MatrixForCell(_endLocation);
    _unit->SetGlobalPosition(rt, nullptr, nullptr, false);
}

void MAXAnimationObject::StartAnimation()
{}
