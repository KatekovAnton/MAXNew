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