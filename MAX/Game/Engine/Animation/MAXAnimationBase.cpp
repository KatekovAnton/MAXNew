//
//  MAXAnimationBase.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationBase.h"
#include "MAXEngine.h"

MAXAnimationBase::MAXAnimationBase()
{
}

MAXAnimationBase::~MAXAnimationBase()
{}


void MAXAnimationBase::Start()
{
    _animStartTime = engine->FullTime();
    this->StartAnimation();
}