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

MAXAnimationObjectUnit::MAXAnimationObjectUnit(const CCPoint& startLocation, const CCPoint& endLocation, MAXUnitObject* object, const MAXANIMATION_CURVE moveCurve)      //creates move action
:MAXAnimationBase(), _unit(object), _startLocation(startLocation), _endLocation(endLocation), _type(MAXANIMATION_UNITMOVE), _firstFlag(true)
{
    _startBodyOffset = _unit->headOffset;
    _moveCurve = moveCurve;
    _aniTime = 0.2;
    if (_moveCurve != MAXANIMATION_CURVE_EASE_LINEAR)
    {
        _aniTime *= M_PI_2;
    }
    _deltaLocation = CCPoint(_endLocation.x - _startLocation.x, _endLocation.y - _startLocation.y);
}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(int bodyIndex, int newBodyIndex, int headIndex, MAXUnitObject* object)                                  //creates rotate action
:MAXAnimationBase(), _unit(object), _bodyIndex(newBodyIndex), _headIndex(headIndex), _type(MAXANIMATION_UNITROTATE), _startHeadIndex(object->GetPureHeadIndex()), _startBodyIndex(bodyIndex), _firstFlag(true)
{
    _aniTime = 0.08 * GetRotateLengt(_startBodyIndex, _bodyIndex);
}

MAXAnimationObjectUnit::MAXAnimationObjectUnit(float firetime, MAXUnitObject* object)                                               //creates fire action
:MAXAnimationBase(), _unit(object), _type(MAXANIMATION_UNITFIRE), _firstFlag(true)
{
    _startBodyOffset = _unit->headOffset;
    _aniTime = firetime;
}

void MAXAnimationObjectUnit::SetMoveFactor(float moveFactor)
{
    _aniTime = 0.02 * moveFactor;
    if (_moveCurve != MAXANIMATION_CURVE_EASE_LINEAR)
    {
        _aniTime *= M_PI_2;
    }
}

int MAXAnimationObjectUnit::CalculateInterpolatedIndex(double theta)
{
    return 0;
}

void MAXAnimationObjectUnit::Update(double time)
{
    double elapsed = (engine->FullTime()-GetStartTime());
    float deltaTime = GetAniElapsedPart(elapsed);
    
    switch (_type)
    {
        case MAXANIMATION_UNITFIRE:
        {
            if (!_unit->IsSingleFire())
            {
                int count = deltaTime/0.2;
                _unit->SetIsFireing(true, count%2 == 1);
                
            }
            else
            {
                if (_unit->params_w->_isInfantry) {
                    if (_firstFlag) {
                        _firstFlag = false;
                        _startBodyOffset = _unit->headOffset;
                    }
                    
                    
                    int stepsFramesCount = 56;
                    stepsFramesCount /= 8;
                    
                    int currentstep = stepsFramesCount * deltaTime;
                    int neededOffsetForCurrentStep = _unit->params_w->stepFrameEnd + 1 + currentstep * 8;
                    _unit->SetBodyOffset(neededOffsetForCurrentStep);
                    
                }
            }
            
        }   break;
            
        case MAXANIMATION_UNITMOVE:
        {
            CCPoint result = CCPoint(_startLocation.x + _deltaLocation.x * deltaTime, _startLocation.y + _deltaLocation.y * deltaTime);
            _unit->SetPosition(result);
            if (_unit->params_w->_isInfantry) {
                if (_firstFlag) {
                    _firstFlag = false;
                    _startBodyOffset = _unit->headOffset;
                }
                int stepsFramesCount = _unit->params_w->stepFrameEnd - _unit->params_w->stepFrameStart + 1;
                stepsFramesCount /= 8;
                stepsFramesCount = stepsFramesCount;
                
                int currentstep = stepsFramesCount * deltaTime;
                int neededOffsetForCurrentStep = _unit->params_w->stepFrameStart + currentstep * 8;
                _unit->SetBodyOffset(neededOffsetForCurrentStep);
                //int steppart =
            }
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            if (_startBodyIndex != _bodyIndex) {
                int index = GetInterpolatedRotateIndex(_startBodyIndex, _bodyIndex, deltaTime);
                if (index != _unit->GetBodyIndex())
                {
                    _unit->SetBodyDirection(index);
                }
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
            if (_unit->params_w->_isInfantry)
                _unit->SetBodyOffset(_startBodyOffset);
        }   break;
    
        case MAXANIMATION_UNITMOVE:
        {
            GLKMatrix4 rt = MAXUnitObject::MatrixForCell(_endLocation);
            _unit->SetGlobalPosition(rt, nullptr, nullptr, false);
            _unit->SetBodyOffset(_startBodyOffset);
        }   break;
            
        case MAXANIMATION_UNITROTATE:
        {
            _unit->SetBodyDirection(_bodyIndex);
            if (_unit->params_w->_hasHead)
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
