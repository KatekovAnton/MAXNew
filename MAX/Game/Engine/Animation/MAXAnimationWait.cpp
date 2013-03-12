//
//  MAXAnimationWait.cpp
//  MAX
//
//  Created by Anton Katekov on 12.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationWait.h"
#include "MAXEngine.h"

MAXAnimationWait::MAXAnimationWait(double waitTime)
:_waitTime(waitTime)
{}

bool MAXAnimationWait::IsFinished()
{
    return MAXAnimationBase::GetStartTime() + _waitTime <= engine->FullTime();
}

void MAXAnimationWait::Update(double time)
{}

void MAXAnimationWait::CompletlyFinish()
{}

void MAXAnimationWait::StartAnimation()
{}