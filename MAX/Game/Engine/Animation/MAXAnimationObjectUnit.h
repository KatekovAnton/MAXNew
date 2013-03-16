//
//  MAXAnimationObjectUnit.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationObjectUnit__
#define __MAX__MAXAnimationObjectUnit__

#include <iostream>
#include "MAXAnimationBase.h"
#include "miniPrefix.h"

class MAXUnitObject;

typedef enum 
{
    MAXANIMATION_UNITMOVE,
    MAXANIMATION_UNITROTATE,
    MAXANIMATION_UNITFIRE
} MAXANIMATION;

class MAXAnimationObjectUnit : public MAXAnimationBase {
    
    int _startBodyIndex;
    int _startHeadIndex;
    
    MAXUnitObject* _unit;
    int _bodyIndex;
    int _headIndex;
    
    double rotateTime;
    
    float _firetime;
    CCPoint _startLocation;
    CCPoint _endLocation;
    
    int _type;
    
    int CalculateInterpolatedIndex(double theta);
    
public:
    
    CCPoint GetStartLocation () const { return _startLocation; };
    CCPoint GetEndLocation () const { return _endLocation; };
    
    MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, MAXUnitObject* object);      //creates move action
    MAXAnimationObjectUnit(int bodyIndex, int headIndex, MAXUnitObject* object);                                  //creates rotate action
    MAXAnimationObjectUnit(double firetime, MAXUnitObject* object);                                               //creates fire action
    
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
    
};

#endif /* defined(__MAX__MAXAnimationObjectUnit__) */
