//
//  MAXAnimationWait.h
//  MAX
//
//  Created by Anton Katekov on 12.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationWait__
#define __MAX__MAXAnimationWait__

#include <iostream>
#include "MAXAnimationBase.h"

class MAXAnimationWait : public MAXAnimationBase {
    
    double _waitTime;
    
public:
    MAXAnimationWait(double waitTime);
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
    
};

#endif /* defined(__MAX__MAXAnimationWait__) */
