//
//  GIUnitParametersNode.h
//  MAX
//
//  Created by  Developer on 17.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIUnitParametersNode__
#define __MAX__GIUnitParametersNode__

#include <iostream>
#include "cocos2d.h"
#include "MAXEngine.h"

using namespace cocos2d;

class GameUnit;

class GIUnitParametersNode: public CCNode
{
    
public:
    GIUnitParametersNode(GameUnit* currentUnit);
    ~GIUnitParametersNode();

};

#endif /* defined(__MAX__UnitParameterNode__) */
