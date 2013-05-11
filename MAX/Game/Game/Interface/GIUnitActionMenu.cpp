//
//  GIUnitActionMenu.cpp
//  MAX
//
//  Created by  Developer on 07.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIUnitActionMenu.h"
#include "MAXContentLoader.h"
#include <vector>
#include "miniPrefix.h"
#include "CocosHelper.h"

#define BUTTON_UNIT_MENU_ACTION_ACTIVATE    "ACTVT"
#define BUTTON_UNIT_MENU_ACTION_ALLOCATE    "ALLOC"
#define BUTTON_UNIT_MENU_ACTION_ATTACK      "ATTK"
#define BUTTON_UNIT_MENU_ACTION_BUILD       "BUILD"
#define BUTTON_UNIT_MENU_ACTION_BUYUPGRADES "BYUPG"
#define BUTTON_UNIT_MENU_ACTION_CLEAR       "CLEAR"
#define BUTTON_UNIT_MENU_ACTION_DISABLE     "DISBL"
#define BUTTON_UNIT_MENU_ACTION_DONE        "DONE"
#define BUTTON_UNIT_MENU_ACTION_ENTER       "ENTER"
#define BUTTON_UNIT_MENU_ACTION_FOLLOW      "FLLOW"
#define BUTTON_UNIT_MENU_ACTION_LOAD        "LOAD"
#define BUTTON_UNIT_MENU_ACTION_PLACE       "PLACE"
#define BUTTON_UNIT_MENU_ACTION_RECHARGE    "RECHG"
#define BUTTON_UNIT_MENU_ACTION_REMOVE      "REMVE"
#define BUTTON_UNIT_MENU_ACTION_REPAIR      "REPAR"
#define BUTTON_UNIT_MENU_ACTION_RELOAD      "RLOAD"
#define BUTTON_UNIT_MENU_ACTION_RESEARCH    "RSRCH"
#define BUTTON_UNIT_MENU_ACTION_SENTRY      "SNTRY"
#define BUTTON_UNIT_MENU_ACTION_START       "START"
#define BUTTON_UNIT_MENU_ACTION_STEAL       "STEAL"
#define BUTTON_UNIT_MENU_ACTION_STOP        "STOP"
#define BUTTON_UNIT_MENU_ACTION_UPGRADE     "UPGRD"
#define BUTTON_UNIT_MENU_ACTION_WAIT        "WAIT"
#define BUTTON_UNIT_MENU_ACTION_XFER        "XFER"
#define BUTTON_UNIT_MENU_ACTION_XFORM       "XFORM"
#define BUTTON_UNIT_MENU_ACTION_INFO        "XFORM"

#define BUTTON_W 38
#define BUTTON_H 21

string imageNameFormType(UNIT_MENU_ACTION action)
{
    string baseName = "";
    switch (action) {
        case UNIT_MENU_ACTION_ACTIVATE:
            baseName = BUTTON_UNIT_MENU_ACTION_ACTIVATE;
            break;
        case UNIT_MENU_ACTION_ALLOCATE:
            baseName = BUTTON_UNIT_MENU_ACTION_ALLOCATE;
            break;
        case UNIT_MENU_ACTION_ATTACK:
            baseName = BUTTON_UNIT_MENU_ACTION_ATTACK;
            break;
        case UNIT_MENU_ACTION_BUILD:
            baseName = BUTTON_UNIT_MENU_ACTION_BUILD;
            break;
        case UNIT_MENU_ACTION_BUYUPGRADES:
            baseName = BUTTON_UNIT_MENU_ACTION_BUYUPGRADES;
            break;
        case UNIT_MENU_ACTION_CLEAR:
            baseName = BUTTON_UNIT_MENU_ACTION_CLEAR;
            break;
        case UNIT_MENU_ACTION_DISABLE:
            baseName = BUTTON_UNIT_MENU_ACTION_DISABLE;
            break;
        case UNIT_MENU_ACTION_DONE:
            baseName = BUTTON_UNIT_MENU_ACTION_DONE;
            break;
        case UNIT_MENU_ACTION_ENTER:
            baseName = BUTTON_UNIT_MENU_ACTION_ENTER;
            break;
        case UNIT_MENU_ACTION_FOLLOW:
            baseName = BUTTON_UNIT_MENU_ACTION_FOLLOW;
            break;
        case UNIT_MENU_ACTION_LOAD:
            baseName = BUTTON_UNIT_MENU_ACTION_LOAD;
            break;
        case UNIT_MENU_ACTION_PLACE:
            baseName = BUTTON_UNIT_MENU_ACTION_PLACE;
            break;
        case UNIT_MENU_ACTION_RECHARGE:
            baseName = BUTTON_UNIT_MENU_ACTION_RECHARGE;
            break;
        case UNIT_MENU_ACTION_REMOVE:
            baseName = BUTTON_UNIT_MENU_ACTION_REMOVE;
            break;
        case UNIT_MENU_ACTION_REPAIR:
            baseName = BUTTON_UNIT_MENU_ACTION_REPAIR;
            break;
        case UNIT_MENU_ACTION_RELOAD:
            baseName = BUTTON_UNIT_MENU_ACTION_RELOAD;
            break;
        case UNIT_MENU_ACTION_RESEARCH:
            baseName = BUTTON_UNIT_MENU_ACTION_RESEARCH;
            break;
        case UNIT_MENU_ACTION_SENTRY:
            baseName = BUTTON_UNIT_MENU_ACTION_SENTRY;
            break;
        case UNIT_MENU_ACTION_START:
            baseName = BUTTON_UNIT_MENU_ACTION_START;
            break;
        case UNIT_MENU_ACTION_STEAL:
            baseName = BUTTON_UNIT_MENU_ACTION_STEAL;
            break;
        case UNIT_MENU_ACTION_STOP:
            baseName = BUTTON_UNIT_MENU_ACTION_STOP;
            break;
        case UNIT_MENU_ACTION_UPGRADE:
            baseName = BUTTON_UNIT_MENU_ACTION_UPGRADE;
            break;
        case UNIT_MENU_ACTION_WAIT:
            baseName = BUTTON_UNIT_MENU_ACTION_WAIT;
            break;
        case UNIT_MENU_ACTION_XFER:
            baseName = BUTTON_UNIT_MENU_ACTION_XFER;
            break;
        case UNIT_MENU_ACTION_XFORM:
            baseName = BUTTON_UNIT_MENU_ACTION_XFORM;
            break;
		case UNIT_MENU_ACTION_INFO:
			baseName = BUTTON_UNIT_MENU_ACTION_INFO;
			break;
            
        default:
            break;
    }
    return baseName;
}

GIUnitActionMenu::GIUnitActionMenu(vector<UNIT_MENU_ACTION> buttons)
{
	_delegate_w = NULL;
    CCArray* arr = CCArray::create();
    initWithArray(arr);
    float delta = 5;
    
    float fullH = buttons.size() * BUTTON_H + (buttons.size()-1)*delta;
    float cH = 0;
    setContentSize(ccz(BUTTON_W, fullH));
    for (int i = 0; i < buttons.size(); i++) {
        UNIT_MENU_ACTION bt = buttons[buttons.size() - i - 1];
        CCMenuItem* item = CreateMenuItemWithType(bt);
        item->setPosition(ccp(0, cH));
        addChild(item);
        cH += BUTTON_H;
        cH += delta;
    }
    setTouchEnabled(true);
    setEnabled(true);
}

GIUnitActionMenu::~GIUnitActionMenu()
{}

CCMenuItem *GIUnitActionMenu::CreateMenuItemWithType(UNIT_MENU_ACTION type)
{
    string baseName = imageNameFormType(type);
    
    Color transparent;
    transparent.r = 0;
    transparent.g = 0;
    transparent.b = 0;
    
    CCSprite* _onSprite = MAXSCL->CreateSpriteFromSimpleImage(baseName + "_ON", transparent);
    CCSprite* _ofSprite = MAXSCL->CreateSpriteFromSimpleImage(baseName + "_OF", transparent);
    
    CCMenuItemSprite* spr = CCMenuItemSprite::create(_ofSprite, _onSprite, this, menu_selector(GIUnitActionMenu::OnButton));
    spr->setContentSize(CCSize(BUTTON_W, BUTTON_H));
    spr->setAnchorPoint(ccp(0, 0));
    spr->setTag(type);
    spr->setEnabled(true);
    CocosHelper::ProceedCCNode(spr);
    
    return spr;
}

void GIUnitActionMenu::OnButton(CCObject* sender)
{
	UNIT_MENU_ACTION action = (UNIT_MENU_ACTION)((CCMenuItem*)sender)->getTag();
    _delegate_w->OnUnitMenuItemSelected(action);
}



