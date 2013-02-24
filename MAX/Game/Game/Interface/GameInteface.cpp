//
//  GameInteface.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameInteface.h"
#include "MAXContetnLoader.h"
#include "Display.h"
#include "CocosHelper.h"

CCMenuItemSprite* createMenuItemFromMaxres(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector)
{
    CCSprite* _onToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(selected);
    CCSprite* _ofToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(normal);
    CCMenuItemSprite* result = CCMenuItemSprite::create(_ofToggleGridSprite, _onToggleGridSprite, target, selector);
    
    CocosHelper::ProceedCCNode(result);
    CCSize sz = result->getContentSize();
    
    CCLabelTTF *label = CCLabelTTF::create(title.c_str(), fontName.c_str(), fontSize);
    CocosHelper::ProceedCCNodeBack(label);
    label->setColor(titleColor);
    CCSize sz1 = label->getContentSize();
    label->setPosition(ccp((sz.width - sz1.width)/2.0, (sz.height - sz1.height)/2.0));
    label->setContentSize(CCSize(title.length()*fontSize, fontSize));
    label->setAnchorPoint(ccp(0.5, 0.5));
    result->addChild(label, 1);

    return result;
}

GameInterface::GameInterface()
{
    _drawGrid = false;
    _drawScan = false;
}

GameInterface::~GameInterface()
{
}

void GameInterface::InitBaseInterface()
{
    _toggleGridButton = createMenuItemFromMaxres("Grid", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleGrid));
    _toggleGridButton->setPosition(ccp(40,280));
    CCMenu *menu = CCMenu::create(_toggleGridButton, nullptr);
    
    _toggleScanButton = createMenuItemFromMaxres("Scan", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleScan));
    _toggleScanButton->setPosition(ccp(40,230));
    menu->addChild(_toggleScanButton);
  //  _toggleRangeButton;
   // _toggleShotsButton;
   // _toggleStatusButton;
    
    
    
    
    menu->setPosition(CCPoint(0, 0));
    menu->setContentSize(CCSize(100, Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale()));
    menu->setTouchEnabled(true);
    this->addChild(menu);
    
    
    UpdateToggleGridButton();
    UpdateToggleScanButton();
}

#pragma mark - Update left buttons

void GameInterface::UpdateToggleGridButton()
{
    if(_drawGrid)
        _toggleGridButton->setNormalImage(CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_ON")));
    else
        _toggleGridButton->setNormalImage(CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_OF")));
}

void GameInterface::UpdateToggleScanButton()
{
    if(_drawScan)
        _toggleScanButton->setNormalImage(CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_ON")));
    else
        _toggleScanButton->setNormalImage(CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_OF")));
}

#pragma mark - Button events
void GameInterface::OnToggleGrid()
{
    _drawGrid = !_drawGrid;
    engine->drawGrid = _drawGrid;
    UpdateToggleGridButton();
}

void GameInterface::OnToggleScan()
{
    _drawScan = !_drawScan;
    engine->drawScan = _drawScan;
    UpdateToggleScanButton();
}

