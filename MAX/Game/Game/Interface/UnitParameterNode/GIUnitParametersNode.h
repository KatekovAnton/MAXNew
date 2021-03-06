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
#include <vector>

using namespace cocos2d;
using namespace std;

class GameUnit;
class GIUnitParameterRow;

class GIUnitParametersNode: public CCNode
{
    
    vector<GIUnitParameterRow*> _rows;
    CCLayerColor* statsFrame;
    
    CCLabelTTF *_labelType;
    
public:
    GIUnitParametersNode(float baseW);
    virtual  ~GIUnitParametersNode();

    
    
    void SetUnit(GameUnit* unit, bool isEnemyUnit);
    void UpdateParameters();
    
    static GIUnitParametersNode* create(float baseW);

};

#endif /* defined(__MAX__UnitParameterNode__) */
