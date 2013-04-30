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
    
    bool _firstFlag;
    int _startBodyOffset;
    
    
    MAXUnitObject* _unit;
    MAXANIMATION _type;

    // Body rotation stuff
    int _startBodyIndex;
    int _bodyIndex;

    // Head rotation stuff - not used now
    int _startHeadIndex;
    int _headIndex;

    // Move stuff
    CCPoint _startLocation;
    CCPoint _endLocation;
    CCPoint _deltaLocation;
    
    
    int CalculateInterpolatedIndex(double theta);
    
public:
    
    CCPoint GetStartLocation () const { return _startLocation; };
    CCPoint GetEndLocation () const { return _endLocation; };
    void SetMoveFactor(float moveFactor);
    
    MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, MAXUnitObject* object, const MAXANIMATION_CURVE moveCurve);      //creates move action
    MAXAnimationObjectUnit(int bodyIndex, int newBodyIndex, int headIndex, MAXUnitObject* object);                                  //creates rotate action
    MAXAnimationObjectUnit(float firetime, MAXUnitObject* object);                                               //creates fire action
    
    virtual void Update(double time);
    virtual void CompletlyFinish();
    virtual void StartAnimation();
    
};

#endif /* defined(__MAX__MAXAnimationObjectUnit__) */
