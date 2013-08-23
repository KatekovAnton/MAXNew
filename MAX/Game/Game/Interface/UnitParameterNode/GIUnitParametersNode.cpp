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
#include "MAXObjectConfig.h"

#include "GameUnit.h"

const float _topStart = 100;

GIUnitParametersNode::GIUnitParametersNode()
{
    statsFrame = CCLayerColor::create(CocosHelper::normalColor(), 66, 109);
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
    
    MAXObjectConfig* config = unit->GetConfig();
    std::vector<UNIT_PARAMETER_TYPE> parameters = config->GetShortParameterList();
    for (int i = 0; i < parameters.size(); i++)
    {
        UNIT_PARAMETER_TYPE param = parameters[i];
        
        GIUnitParameterRow* row1 = GIUnitParameterRow::create();
        addChild(row1);
        row1->setPosition(ccp(0, _topStart-GIUnitParameterRow::RowHeight() * (i + 1)));
        row1->SetImageForParameterType(param, 0);
        row1->SetMaxValue(unit->GetParameterMaxValue(param));
        row1->SetCurrentValue(unit->GetParameterValue(param));
        _rows.push_back(row1);
    }
    if (_rows.size() > 0) {
        _rows[_rows.size()-1]->SetSeparatorVisible(false);
    }
//    Strached sprite 9 is needed
//    CCSize s = statsFrame->getContentSize();
//    s.height = 10 + parameters.size() * GIUnitParameterRow::RowHeight();
//    statsFrame->setContentSize(s);
}

void GIUnitParametersNode::UpdateParameters()
{}

GIUnitParametersNode* GIUnitParametersNode::create()
{
    GIUnitParametersNode* result = new GIUnitParametersNode();
    result->autorelease();
    
    
    result->setContentSize(ccz(68, 110));
    result->setAnchorPoint(ccp(0, 0));
    
    return result;
}



