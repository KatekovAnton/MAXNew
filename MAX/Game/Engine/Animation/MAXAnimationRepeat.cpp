//
//  MAXAnimationRepeat.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationRepeat.h"

MAXAnimationRepeat::MAXAnimationRepeat(MAXAnimationBase* animation)
{
    _animaton = animation;
}

MAXAnimationRepeat::~MAXAnimationRepeat()
{
    delete _animaton;
}
    
bool MAXAnimationRepeat::IsFinished()
{
    return false;
}

void MAXAnimationRepeat::Update(double time)
{
    if (_animaton->IsFinished())
        _animaton->BaseStart();
}

void MAXAnimationRepeat::CompletlyFinish()
{
    _animaton->CompletlyFinish();
}

void MAXAnimationRepeat::StartAnimation()
{
    _animaton->StartAnimation();
}