//
//  MAXAnimationEffect.h
//  MAX
//
//  Created by  Developer on 10.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationEffect__
#define __MAX__MAXAnimationEffect__

#include <iostream>
#include "MAXAnimationBase.h"
#include "miniPrefix.h"

class MAXEffectObject;

class MAXAnimationEffect : public MAXAnimationBase {
    
    MAXEffectObject* _effect;
    
public:
    
    MAXAnimationEffect(MAXEffectObject* effect);
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
};

#endif /* defined(__MAX__MAXAnimationEffect__) */
