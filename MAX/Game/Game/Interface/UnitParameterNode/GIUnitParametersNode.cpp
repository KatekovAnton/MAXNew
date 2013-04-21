//
//  GIUnitParametersNode.cpp
//  MAX
//
//  Created by  Developer on 17.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameInterfacePrefix.h"

#include "GIUnitParametersNode.h"
#include "GIUnitParameterRow.h"
#include "CocosHelper.h"
#include "StringUtils.h"

#include "GameUnit.h"

const float _topStart = 100;

GIUnitParametersNode::GIUnitParametersNode()
{
    CCSprite* statsFrame = CCSprite::create("statsFrame.png");
    statsFrame->setAnchorPoint(ccp(0, 0));
    statsFrame->setPosition(ccp(0, 0));
    addChild(statsFrame);
}

GIUnitParametersNode::~GIUnitParametersNode()
{}

void GIUnitParametersNode::SetUnit(GameUnit* unit)
{
    for (int i = 0; i < _rows.size(); i++) {
        _rows[i]->removeFromParentAndCleanup(true);
    }
    _rows.clear();
    if (!unit)
        return;
    
    GIUnitParameterRow* row1 = GIUnitParameterRow::create();
    addChild(row1);
    row1->setPosition(ccp(0, _topStart-GIUnitParameterRow::RowHeight()));
    row1->SetImageForParameterType(UNIT_PARAMETER_TYPE_ENERGY, 0);
    row1->SetMaxValue(1200);
    row1->SetCurrentValue(1086);
    _rows.push_back(row1);
}

void UpdateParameters()
{}

GIUnitParametersNode* GIUnitParametersNode::create()
{
    GIUnitParametersNode* result = new GIUnitParametersNode();
    result->autorelease();
    
    
    result->setContentSize(ccz(64, 111));
    result->setAnchorPoint(ccp(0, 0));
    
    return result;
}