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

typedef enum
{
    MAXANIMATION_CURVE_EASE_IN = 1,
    MAXANIMATION_CURVE_EASE_OUT = 2,
    MAXANIMATION_CURVE_EASE_IN_OUT = 3,
    MAXANIMATION_CURVE_EASE_LINEAR = 4
} MAXANIMATION_CURVE;

class MAXAnimationBase {
    
    double _animStartTime;
public:

    float _aniTime;
    MAXANIMATION_CURVE _moveCurve;
    
    bool _isCompletlyFinish;
    
    MAXAnimationDelegate* _delegate;
    
    double GetStartTime() const {return _animStartTime;};
    
    MAXAnimationBase();
    virtual ~MAXAnimationBase();
    
    float GetAniElapsedPart(double elapsedTime);
    
    void BaseStart();
    void BaseCompletlyFinish();
    void BaseUpdate(double time);
    
    virtual bool UpdatesPosition();
    
    virtual bool IsFinished();
    virtual void Update(double time) = 0;
    virtual void CompletlyFinish() = 0;
    virtual void StartAnimation() = 0;
    
};

#endif /* defined(__MAX__MAXAnimationObjectBase__) */
