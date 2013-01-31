//
//  MAXAnimationSequence.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationSequence.h"

MAXAnimationSequence::MAXAnimationSequence()
:MAXAnimationBase(), _lastCompletlyFinishAnimation(0)
{
    
}

MAXAnimationSequence::~MAXAnimationSequence()
{
    for (int i = 0; i < _animations.size(); i++)
    {
        MAXAnimationBase* anim = _animations.at(i);
        delete anim;
    }
}

MAXAnimationBase* MAXAnimationSequence::CurrentAnimaton()
{
    for (int i = 0; i < _animations.size(); i++)
    {
        MAXAnimationBase* anim = _animations.at(i);
        if (!anim->IsFinished())
            return anim;
        else if (_lastCompletlyFinishAnimation < i)
        {
            anim->CompletlyFinish();
            _lastCompletlyFinishAnimation++;
        }
    }
    return nil;
}

void MAXAnimationSequence::AddAnimation(MAXAnimationBase* baseAnim)
{
    _animations.push_back(baseAnim);
}

bool MAXAnimationSequence::IsFinished()
{
    MAXAnimationBase* anim = _animations.at(_animations.size());
    return anim->IsFinished();
}

void MAXAnimationSequence::Update(double time)
{
    MAXAnimationBase* anim = this->CurrentAnimaton();
    if (anim)
        anim->Update(time);
}

void MAXAnimationSequence::CompletlyFinish()
{
    
}
