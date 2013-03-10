//
//  MAXAnimationEffect.cpp
//  MAX
//
//  Created by  Developer on 10.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationEffect.h"
#include "MAXEffectObject.h"
#include "MAXEngine.h"

const double frameTime = 0.15;

MAXAnimationEffect::MAXAnimationEffect(MAXEffectObject* effect)
:_effect(effect)
{}

bool MAXAnimationEffect::IsFinished()
{
    return MAXAnimationBase::GetStartTime() + _effect->_frameCount * frameTime <= engine->FullTime();
}

void MAXAnimationEffect::Update(double time)
{
    double startAnimTime = MAXAnimationBase::GetStartTime();
    double elapsed = engine->FullTime() - startAnimTime;
    int frame = elapsed/frameTime;
    _effect->SetEffectFrame(frame);
}

void MAXAnimationEffect::CompletlyFinish()
{
    _effect->SetEffectFrame(_effect->_frameCount - 1);
}

void MAXAnimationEffect::StartAnimation()
{
    _effect->SetEffectFrame(0);
}