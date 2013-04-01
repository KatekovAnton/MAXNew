//
//  MAXAnimationBase.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationBase.h"
#include "MAXEngine.h"
#include "MAXAnimationDelegate.h"

MAXAnimationBase::MAXAnimationBase()
:_delegate(NULL), _isCompletlyFinish(false)
{
}

MAXAnimationBase::~MAXAnimationBase()
{}

void MAXAnimationBase::BaseUpdate(double time)
{
    this->Update(time);
    if (_delegate)
        _delegate->OnAnimationUpdate(this);
    
}

float MAXAnimationBase::GetAniElapsedPart(double elapsedTime)
{
    float result = 0;
    if (_moveCurve == MAXANIMATION_CURVE_EASE_IN_OUT)
    {
        float fromminonetoone = (2.0*elapsedTime/_aniTime) - 1.0;
        result = sinf(fromminonetoone * M_PI_2) * 0.5 + 0.5;
    }
    else if (_moveCurve == MAXANIMATION_CURVE_EASE_IN)
    {
        float fromminonetoone = (elapsedTime/_aniTime) - 1.0;
        result = sinf(fromminonetoone * M_PI_2) * 1.0 + 1.0;
    }
    else if (_moveCurve == MAXANIMATION_CURVE_EASE_OUT)
    {
        float fromminonetoone = (elapsedTime/_aniTime);
        result = sinf(fromminonetoone * M_PI_2) * 1.0;
    }
    else //if (_moveCurve == MAXANIMATION_CURVE_EASE_LINEAR)
    {
        result = elapsedTime/_aniTime;
    }
    return result;
}

void MAXAnimationBase::BaseStart()
{
    _animStartTime = engine->FullTime();
    this->StartAnimation();
    if (_delegate) 
        _delegate->OnAnimationStart(this);
    
}

void MAXAnimationBase::BaseCompletlyFinish()
{
    if(_isCompletlyFinish)
        return;
    _isCompletlyFinish = true;
    this->CompletlyFinish();
    if (_delegate) 
        _delegate->OnAnimationFinish(this);
    
}

bool MAXAnimationBase::IsFinished()
{
    return GetStartTime() + _aniTime <= engine->FullTime();
}
