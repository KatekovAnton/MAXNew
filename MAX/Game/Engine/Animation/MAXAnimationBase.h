//
//  MAXAnimationBase.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationBase__
#define __MAX__MAXAnimationBase__

#include <iostream>

class MAXAnimationDelegate;

class MAXAnimationBase {
    
    double _animStartTime;
public:
    
    bool _isCompletlyFinish;
    
    MAXAnimationDelegate* _delegate;
    
    double GetStartTime() const {return _animStartTime;};
    
    MAXAnimationBase();
    virtual ~MAXAnimationBase();
    
    
    void BaseStart();
    void BaseCompletlyFinish();
    void BaseUpdate(double time);
    
    virtual bool IsFinished() = 0;
    virtual void Update(double time) = 0;
    virtual void CompletlyFinish() = 0;
    virtual void StartAnimation() = 0;
    
};

#endif /* defined(__MAX__MAXAnimationObjectBase__) */
