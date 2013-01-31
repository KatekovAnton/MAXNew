//
//  MAXAnimationObjectUnit.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationObjectUnit.h"
#include "MAXUnitObject.h"

const float fireTime = 0.15;
const float rotateRime = 0.1;

MAXAnimationObjectUnit::MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, const shared_ptr<MAXUnitObject>& object)      //creates move action
:_unit(object), _startLocation(startLocation), _endLocation(endLocation)
{}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(int bodyIndex, int headIndex, const shared_ptr<MAXUnitObject>& object)                                  //creates rotate action
:_unit(object), _bodyIndex(bodyIndex), _headIndex(headIndex)
{

}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(double firetime, const shared_ptr<MAXUnitObject>& object)                                               //creates fire action
:_unit(object), _firetime(firetime)
{}

bool MAXAnimationObjectUnit::IsFinished()
{
    return false;
}

void MAXAnimationObjectUnit::Update(double time)
{
    switch (_type)
    {
        case MAXANIMATION_UNITFIRE:
        {
            
        }   break;
            
        case MAXANIMATION_UNITMOVE:
        {
           
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            
        }   break;
        default:
            break;
    }
}

void MAXAnimationObjectUnit::CompletlyFinish()
{
    switch (_type)
    {
        case MAXANIMATION_UNITFIRE:
        {
            _unit->SetIsFireing(false);
        }   break;
    
        case MAXANIMATION_UNITMOVE:
        {
            _unit->SetBodyDirection(MAXUnitObject::CalculateImageIndex(_startLocation, _endLocation));
            GLKMatrix4 rt = MAXUnitObject::MatrixForCell(_endLocation);
            _unit->SetGlobalPosition(rt, nullptr, nullptr, false);
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            _unit->SetBodyDirection(_bodyIndex);
            _unit->SetHeadDirection(_headIndex);
        }   break;
        default:
            break;
    }
}

