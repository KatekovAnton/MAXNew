//
//  GIUnitSelectionMenu.cpp
//  MAX
//
//  Created by Anton Katekov on 19.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIUnitSelectionMenu.h"
#include "GameUnit.h"
#include <vector>
#include "miniPrefix.h"
#include "GameInterfacePrefix.h"
#include "CocosHelper.h"
#include "MAXObjectConfig.h"
#include "MAXContentLoader.h"
#include "GameMatchPlayer.h"
#include "GameUnitData.h"

#define BUTTON_W 64
#define BUTTON_H 64
#define BUTTON_LABEL_TAG 11

GIUnitSelectionMenu::GIUnitSelectionMenu(const vector<GameUnit*> units, GameUnit *currentUnit, const CCPoint &point)
:_units(units), _delegate_w(NULL), _point(point)
{
    CCArray* arr = CCArray::create();
    initWithArray(arr);
    
    float delta = 1;
    
    float fullH = units.size() * BUTTON_H + (units.size()-1)*delta;
    float cH = 0;
    setContentSize(ccz(BUTTON_W, fullH));
    for (int i = 0; i < units.size(); i++) {
        GameUnit* bt = units[i];
        CCMenuItem* item = CreateMenuItemWithUnit(bt, i, currentUnit);
        item->setPosition(ccp(0, cH));
        addChild(item);
        cH += BUTTON_H;
        cH += delta;
    }
    setTouchEnabled(true);
    setEnabled(true);
}

GIUnitSelectionMenu::~GIUnitSelectionMenu()
{}

CCMenuItem *GIUnitSelectionMenu::CreateMenuItemWithUnit(GameUnit *unit, int index, GameUnit *currentUnit)
{
    Color transparent;
    transparent.r = 0;
    transparent.g = 0;
    transparent.b = 0;
    
    CCSprite* _onSprite = CCSprite::create("selectUnitMenuBG.png");// MAXSCL->CreateSpriteFromSimpleImage(baseName + "_ON", transparent);
    CCSprite* _ofSprite = CCSprite::create("selectUnitMenuBG.png");//MAXSCL->CreateSpriteFromSimpleImage(baseName + "_OF", transparent);
    
    CCMenuItemSprite* spr = CCMenuItemSprite::create(_ofSprite, _onSprite, this, menu_selector(GIUnitSelectionMenu::OnButton));
    spr->setContentSize(CCSize(BUTTON_W, BUTTON_H));
    spr->setAnchorPoint(ccp(0, 0));
    spr->setTag(index);
    spr->setEnabled(true);
    
    MAXObjectConfig *config = unit->GetConfig();
    CCSprite* image = CCSprite::createWithTexture(unit->_owner_w->CreateTexture2DFromMaterialFirstFrame(config->_smallName));
    spr->addChild(image);
    CCSize size = image->getContentSize();
    float scaleH = 30.0 / size.height;
    float scaleW = 50.0 / size.width;
    float scale = MIN(scaleH, scaleW);
    
    image->setScale(scale);
    image->setPosition(ccp(32, 40));
    
    float lineFullW = 54;
    float greenW = (float)unit->_unitData->GetParameterValue(UNIT_PARAMETER_TYPE_HEALTH) / (float)unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_HEALTH) * lineFullW;
    float redW = (float)unit->_unitData->GetDamageValue(currentUnit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_ATTACK), 1) / (float)unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_HEALTH) * lineFullW;
    if (redW > greenW)
        redW = greenW;
    
    
    CCLayerColor *white = CCLayerColor::create({255,255,255,255}, lineFullW + 2, 14);
    spr->addChild(white);
    white->setPosition(ccp(4, 4));
    
    CCLayerColor *black = CCLayerColor::create({0,0,0,255}, lineFullW, 12);
    spr->addChild(black);
    black->setPosition(ccp(5, 5));
    
    CCLayerColor *green = CCLayerColor::create({0,255,0,255}, greenW, 12);
    spr->addChild(green);
    green->setPosition(ccp(5, 5));
    
    CCLayerColor *red = CCLayerColor::create({255,0,0,255}, redW, 12);
    spr->addChild(red);
    red->setPosition(ccp(5 + greenW - redW, 5));
    
    return spr;
}

void GIUnitSelectionMenu::OnButton(CCMenuItem* sender)
{
    if (_delegate_w)
        _delegate_w->OnUnitSelected(_units[sender->getTag()], _point);
}


