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

#define BUTTON_W 64
#define BUTTON_H 64
#define BUTTON_LABEL_TAG 11

GIUnitSelectionMenu::GIUnitSelectionMenu(const vector<GameUnit*> units)
:_units(units), _delegate_w(NULL)
{
    CCArray* arr = CCArray::create();
    initWithArray(arr);
    
    float delta = 1;
    
    float fullH = units.size() * BUTTON_H + (units.size()-1)*delta;
    float cH = 0;
    setContentSize(ccz(BUTTON_W, fullH));
    for (int i = 0; i < units.size(); i++) {
        GameUnit* bt = units[i];
        CCMenuItem* item = CreateMenuItemWithUnit(bt, i);
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

CCMenuItem *GIUnitSelectionMenu::CreateMenuItemWithUnit(GameUnit *unit, int index)
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
    
    return spr;
}

void GIUnitSelectionMenu::OnButton(CCObject* sender)
{}

