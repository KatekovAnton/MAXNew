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
    
public:
    
    CCPoint GetStartLocation () const { return _startLocation; };
    CCPoint GetEendLocation () const { return _endLocation; };
    
    MAXAnimationObject(const CCPoint& startLocation, const CCPoint& endLocation, MAXObject* object);      //creates move action
    
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
    
};

#endif /* defined(__MAX__MAXAnimationObject__) */
