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

enum MAXGAMESTATE
{
    MAXANIMATION_UNITMOVE,
    MAXANIMATION_UNITROTATE,
    MAXANIMATION_UNITFIRE
};

class MAXAnimationObjectUnit : public MAXAnimationBase {
    
    shared_ptr<MAXUnitObject> _unit;
    int _bodyIndex;
    int _headIndex;
    float _firetime;
    CCPoint _startLocation;
    CCPoint _endLocation;
    
    int _type;
    
public:
    
    MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, const shared_ptr<MAXUnitObject>& object);      //creates move action
    MAXAnimationObjectUnit(int bodyIndex, int headIndex, const shared_ptr<MAXUnitObject>& object);                                  //creates rotate action
    MAXAnimationObjectUnit(double firetime, const shared_ptr<MAXUnitObject>& object);                                               //creates fire action
    
    
    virtual bool IsFinished();
    virtual void Update(double time);
    virtual void CompletlyFinish();
    
};

#endif /* defined(__MAX__MAXAnimationObjectUnit__) */
