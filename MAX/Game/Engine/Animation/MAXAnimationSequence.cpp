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
            anim->BaseCompletlyFinish();
            _lastCompletlyFinishAnimation++;
            if (i<_animations.size()-2) {
                MAXAnimationBase* anim1 = _animations.at(i+1);
                anim1->BaseStart();
            }
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
    if (_animations.size() == 0)
        return true;
    MAXAnimationBase* anim = _animations.at(_animations.size() - 1);
    return anim->IsFinished();
}

void MAXAnimationSequence::Update(double time)
{
    MAXAnimationBase* anim = this->CurrentAnimaton();
    if (anim)
        anim->BaseUpdate(time);
}

void MAXAnimationSequence::CompletlyFinish()
{
    if (_animations.size() == 0)
        return;
    MAXAnimationBase* anim = _animations.at(_animations.size() - 1);
    return anim->BaseCompletlyFinish();
}

void MAXAnimationSequence::StartAnimation()
{
    if (_animations.size() == 0)
        return;
    _animations.at(0)->BaseStart();
}