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

void ProceedCCNode(CCNode* node)
{
    node->setScale(Display::currentDisplay()->GetDisplayScale());
    node->setAnchorPoint(ccp(0, 0));
}

GameInterface::GameInterface()
{
    
}

GameInterface::~GameInterface()
{
}

void GameInterface::InitBaseInterface()
{
    CCSprite* _onToggleGridSprite = CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_ON"));
    CCSprite* _ofToggleGridSprite = CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_OF"));
    
    _toggleGridButton = CCMenuItemSprite::create(_ofToggleGridSprite, _onToggleGridSprite, this, menu_selector(GameInterface::OnToggleGrid));
    ProceedCCNode(_toggleGridButton);
    
    CCMenu *menu = CCMenu::create(_toggleGridButton, nullptr);
    menu->setPosition(CCPoint(40, 280));
    menu->setTouchEnabled(true);
    this->addChild(menu);
    
    
    UpdateToggleGridButton();
}

#pragma mark - Button events
void GameInterface::OnToggleGrid()
{
    _drawGrid = !_drawGrid;
    engine->drawGrid = _drawGrid;
    UpdateToggleGridButton();
}

void GameInterface::UpdateToggleGridButton()
{
    if(_drawGrid)
        _toggleGridButton->setNormalImage(CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_ON")));
    else
        _toggleGridButton->setNormalImage(CCSprite::createWithTexture(MAXSCL->CreateTexture2DFromSimpleImage("AMMO_OF")));
}