//
//  GIUnitParameterRow.h
//  MAX
//
//  Created by  Developer on 17.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIUnitParameterRow__
#define __MAX__GIUnitParameterRow__

#include <iostream>
#include "cocos2d.h"
#include "MAXEngine.h"
#include "miniPrefix.h"

using namespace cocos2d;

class GameUnitCurrentState;

class GIUnitParameterRow: public CCNode
{
    CCLabelTTF* _labelValue;
    CCLabelTTF* _labelDiscr;
    CCSprite* _imageFull;
    
    int _value;
    int _maxValue;
    
public:
    
    GIUnitParameterRow();
    ~GIUnitParameterRow();
    
    void SetImageForParameterType(UNIT_PARAMETER_TYPE type, int subtype);
    void SetCurrentValue(int value);
    void SetMaxValue(int value);
    void SetTextColor(GIPARAMETER_COLOR color);
    
    static GIUnitParameterRow* create();
    
    static float RowHeight(){return 12;}
    
};

#endif /* defined(__MAX__ParameterRow__) */