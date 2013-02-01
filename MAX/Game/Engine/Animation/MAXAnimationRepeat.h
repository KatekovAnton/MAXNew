//
//  MAXAnimationRepeat.h
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationRepeat__
#define __MAX__MAXAnimationRepeat__

#include <iostream>
#include "MAXAnimationBase.h"
#include "miniPrefix.h"

class MAXAnimationRepeat : public MAXAnimationBase {
    
    MAXAnimationBase* _animaton;
    
public:
    
    MAXAnimationRepeat(MAXAnimationBase* animation);
    ~MAXAnimationRepeat();
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
};

#endif /* defined(__MAX__MAXAnimationRepeat__) */
