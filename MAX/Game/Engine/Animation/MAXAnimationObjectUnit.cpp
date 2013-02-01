//
//  MAXAnimationObjectUnit.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationObjectUnit.h"
#include "MAXUnitObject.h"
#include "MAXEngine.h"

const float rotateRime = 0.1;

MAXAnimationObjectUnit::MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, const shared_ptr<MAXUnitObject>& object)      //creates move action
:_unit(object), _startLocation(startLocation), _endLocation(endLocation), _type(MAXANIMATION_UNITMOVE)
{}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(int bodyIndex, int headIndex, const shared_ptr<MAXUnitObject>& object)                                  //creates rotate action
:_unit(object), _bodyIndex(bodyIndex), _headIndex(headIndex), _type(MAXANIMATION_UNITROTATE)
{}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(double firetime, const shared_ptr<MAXUnitObject>& object)                                               //creates fire action
:_unit(object), _firetime(firetime), _type(MAXANIMATION_UNITFIRE)
{}


int MAXAnimationObjectUnit::CalculateInterpolatedIndex(double theta)
{
    return 0;
}

bool MAXAnimationObjectUnit::IsFinished()
{
    switch (_type)
    {
        case MAXANIMATION_UNITFIRE:
        {
            return MAXAnimationBase::GetStartTime() + _firetime <= engine->FullTime();
        }   break;
            
        case MAXANIMATION_UNITMOVE:
        {
            return true;
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            return true;
        }   break;
        default:
            break;
    }
    return true;
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

void MAXAnimationObjectUnit::StartAnimation()
{
    switch (_type)
    {
        case MAXANIMATION_UNITFIRE:
        {
            _unit->SetIsFireing(true);
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
