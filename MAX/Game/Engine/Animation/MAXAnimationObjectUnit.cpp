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
#include "MAXUnitMaterial.h"


int GetInterpolatedRotateIndex(int from, int to, float zerotoone)
{
    if (from > to)
    {
        int lengthRight = from - to; 
        int lengthReverse = to + 8 - from;
        
        if (lengthRight < lengthReverse)
            return from - ((float)lengthRight * zerotoone);
        else
            return (from + (int)((float)lengthReverse * zerotoone))%8;
    }
    else //if (to > from)
    {
        int lengthRight = to - from;
        int lengthReverse = from + 8 - to;
        if (lengthRight < lengthReverse)
            return from + ((float)lengthRight)*zerotoone;
        else
            return (8+from - (int)((float)lengthReverse * zerotoone))%8;
    }
}

int GetRotateLengt(int from, int to)
{
    if (from > to)
    {
        int lengthRight = from - to;
        int lengthReverse = to + 8 - from;
        
        if (lengthRight < lengthReverse)
            return lengthRight;
        else
            return lengthReverse;
    }
    else //if (to > from)
    {
        int lengthRight = to - from;
        int lengthReverse = from + 8 - to;
        if (lengthRight < lengthReverse)
            return lengthRight;
        else
            return lengthReverse;
    }
}

const float moveTime = 0.2;

MAXAnimationObjectUnit::MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, const shared_ptr<MAXUnitObject>& object)      //creates move action
:MAXAnimationBase(), _unit(object), _startLocation(startLocation), _endLocation(endLocation), _type(MAXANIMATION_UNITMOVE)
{}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(int bodyIndex, int headIndex, const shared_ptr<MAXUnitObject>& object)                                  //creates rotate action
:MAXAnimationBase(), _unit(object), _bodyIndex(bodyIndex), _headIndex(headIndex), _type(MAXANIMATION_UNITROTATE), _startHeadIndex(object->GetPureHeadIndex()), _startBodyIndex(object->GetBodyIndex()), rotateTime(0.07 * GetRotateLengt(_startBodyIndex, _bodyIndex))
{}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(double firetime, const shared_ptr<MAXUnitObject>& object)                                               //creates fire action
:MAXAnimationBase(), _unit(object), _firetime(firetime), _type(MAXANIMATION_UNITFIRE)
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
            return MAXAnimationBase::GetStartTime() + moveTime <= engine->FullTime();
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            return MAXAnimationBase::GetStartTime() + rotateTime <= engine->FullTime();
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
            if (!_unit->IsSingleFire())
            {
                double elapsed = (engine->FullTime()-GetStartTime())/_firetime;
                int count = elapsed/0.2;
                _unit->SetIsFireing(true, count%2 == 1);
            }
            
        }   break;
            
        case MAXANIMATION_UNITMOVE:
        {
            double elapsed = (engine->FullTime()-GetStartTime());
            float fromminonetoone = (2.0*elapsed/moveTime) - 1.0;
            float deltaTime = sinf(fromminonetoone * M_PI_2) * 0.5 + 0.5;
            CCPoint delta = CCPoint(_endLocation.x - _startLocation.x, _endLocation.y - _startLocation.y);
            CCPoint result = CCPoint(_startLocation.x + delta.x * deltaTime, _startLocation.y + delta.y * deltaTime);
            _unit->SetPosition(result);
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            //printf("updaterotate\n");
            float elapsed = (engine->FullTime()-GetStartTime())/rotateTime;
            
            if (_startBodyIndex != _bodyIndex) {
                int index = GetInterpolatedRotateIndex(_startBodyIndex, _bodyIndex, elapsed);
                _unit->SetBodyDirection(index);
            }
            if (_startHeadIndex != _headIndex) {
                printf("rh\n");
            }
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
            _unit->SetIsFireing(false, false);
        }   break;
    
        case MAXANIMATION_UNITMOVE:
        {
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
            _unit->SetIsFireing(true, true);
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
