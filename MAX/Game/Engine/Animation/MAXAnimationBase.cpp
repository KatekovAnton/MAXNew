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
:_animStartTime(engine->FullTime())
{
}

MAXAnimationBase::~MAXAnimationBase()
{}