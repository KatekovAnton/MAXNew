//
//  MAXAnimationSequence.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationSequence.h"

MAXAnimationSequence::MAXAnimationSequence()
:MAXAnimationBase(), _current(NULL)
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
    }
    return NULL;
}

void MAXAnimationSequence::AddAnimation(MAXAnimationBase* baseAnim)
{
    _animations.push_back(baseAnim);
}

bool MAXAnimationSequence::IsFinished()
{
    if (_animations.size() == 0)
        return true;

    for (int i = 0; i < _animations.size(); i++)
    {
        MAXAnimationBase* anim = _animations.at(i);
        if (!anim->IsFinished())
            return false;
    }
    return true;
}

int MAXAnimationSequence::IndexOfAnimation(MAXAnimationBase* element)
{
    for (int i = 0; i < _animations.size(); i++) {
        if (_animations.at(i) == element) {
            return i;
        }
    }
    return -1;
}

void MAXAnimationSequence::Update(double time)
{
    if (_current->IsFinished())
    {
        _current->BaseCompletlyFinish();
        int index = IndexOfAnimation(_current);
        if (index == _animations.size() - 1)
            return;
        _current = _animations.at(index + 1);
        _current->BaseStart();
        _current->BaseUpdate(time);
        return;
    }
    else
        _current->BaseUpdate(time);
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
    MAXAnimationBase* canim = _animations.at(0);
    canim->BaseStart();
    _current = canim;
}