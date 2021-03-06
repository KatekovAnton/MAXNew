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

class MAXAnimationObject : public MAXAnimationBase {
    
    MAXObject* _unit;
    CCPoint _startLocation;
    CCPoint _endLocation;
    CCPoint _deltaLocation;
    
public:
    
    CCPoint GetStartLocation () const { return _startLocation; };
    CCPoint GetEndLocation () const { return _endLocation; };
    
    MAXAnimationObject(const CCPoint& startLocation, const CCPoint& endLocation, MAXObject* object, float speedMultiplier);      //creates move action
    
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
    
};

#endif /* defined(__MAX__MAXAnimationObject__) */
