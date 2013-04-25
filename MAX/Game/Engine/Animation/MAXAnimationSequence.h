//
//  MAXAnimationSequence.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationSequence__
#define __MAX__MAXAnimationSequence__

#include <iostream>
#include "MAXAnimationBase.h"
#include "miniPrefix.h"



class MAXAnimationSequence : public MAXAnimationBase {
    
    vector<MAXAnimationBase*> _animations;
    int _lastCompletlyFinishAnimation;
    
    MAXAnimationBase* _current;
    
    MAXAnimationBase* CurrentAnimaton();
    int IndexOfAnimation(MAXAnimationBase* element);
    bool _isStoped;
public:
    
    MAXAnimationSequence();
    ~MAXAnimationSequence();
    
    void AddAnimation(MAXAnimationBase* baseAnim);
    void Stop();

    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
};

#endif /* defined(__MAX__MAXAnimationSequence__) */
