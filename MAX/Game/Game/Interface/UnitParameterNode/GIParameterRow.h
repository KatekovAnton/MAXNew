//
//  GIParameterRow.h
//  MAX
//
//  Created by  Developer on 17.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIParameterRow__
#define __MAX__GIParameterRow__

#include <iostream>
#include "cocos2d.h"
#include "MAXEngine.h"
#include "miniPrefix.h"

using namespace cocos2d;

class GameUnitCurrentState;

class GIParameterRow: public CCNode
{
    
public:
    GIParameterRow(GameUnitCurrentState* state, UNIT_PARAMETER_TYPE parameterType);
    ~GIParameterRow();
    
};

#endif /* defined(__MAX__ParameterRow__) */
