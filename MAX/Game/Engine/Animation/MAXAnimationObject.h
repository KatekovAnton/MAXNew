//
//  MAXAnimationObject.h
//  MAX
//
//  Created by  Developer on 10.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationObject__
#define __MAX__MAXAnimationObject__

#include <iostream>
#include "MAXAnimationBase.h"
#include "miniPrefix.h"

class MAXObject;

typedef enum
{
    MAXANIMATION_CURVE_EASE_IN = 1,
    MAXANIMATION_CURVE_EASE_OUT = 2,
    MAXANIMATION_CURVE_EASE_IN_OUT = 3,
    MAXANIMATION_CURVE_EASE_LINEAR = 4
} MAXANIMATION_CURVE;

class MAXAnimationObject : public MAXAnimationBase {
    
    MAXObject* _unit;
    CCPoint _startLocation;
    CCPoint _endLocation;
    
public:
    
    int _moveCurve;
    float _moveTime;
    
    CCPoint GetStartLocation () const { return _startLocation; };
    CCPoint GetEendLocation () const { return _endLocation; };
    
    MAXAnimationObject(const CCPoint& startLocation, const CCPoint& endLocation, MAXObject* object);      //creates move action
    
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
    
};

#endif /* defined(__MAX__MAXAnimationObject__) */