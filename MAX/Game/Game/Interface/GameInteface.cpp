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
#include "GameUnit.h"
#include "MAXEngine.h"
#include "MAXStatusRenderer.h"

float Scale = 1.0;

CCMenuItemSprite* createMenuItemFromMaxres(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector)
{
    CCSprite* _onToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(selected);
    CCSprite* _ofToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(normal);
    CCMenuItemSprite* result = CCMenuItemSprite::create(_ofToggleGridSprite, _onToggleGridSprite, target, selector);
    result->setAnchorPoint(ccp(0, 0));
    
    
    CCLabelTTF *label = CCLabelTTF::create(title.c_str(), fontName.c_str(), fontSize);
    label->setColor(titleColor);
    CCSize sz = result->getContentSize();
    sz.height /= 2;
    label->setPosition(ccp(sz.width * 0.25, sz.height * 0.25));
    sz.width *= Scale;
    sz.height *= Scale;
    label->setContentSize(sz);
    label->setAnchorPoint(ccp(0, 0));
    
    CocosHelper::ProceedCCNode(result);
    CocosHelper::ProceedCCNodeBack(label);
    
    result->addChild(label, 1);

    return result;
}

bool GameInterface::ShouldReceiveTouch(int x, int y) const
{
    CCRect r = CCRect(0, 0, 70, 320);
    return !r.containsPoint(CCPoint(x, y));
}

GameInterface::GameInterface()
{
    _lockUnits = false;
    
    _drawGrid = false;
    _drawScan = false;
   
    _drawRange = false;
    _drawShots = false;
    _drawStatus = false;
    
    Scale = Display::currentDisplay()->GetDisplayScale();
}

GameInterface::~GameInterface()
{
}

void GameInterface::InitBaseInterface()
{
    float bx = 10;
    
    _toggleGridButton = createMenuItemFromMaxres("Grid", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleGrid));
    _toggleGridButton->setPosition(ccp(bx,280));
    CCMenu *menu = CCMenu::create(_toggleGridButton, nullptr);
    
    _toggleScanButton = createMenuItemFromMaxres("Scan", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleScan));
    _toggleScanButton->setPosition(ccp(bx,250));
    menu->addChild(_toggleScanButton);
    
    _toggleRangeButton = createMenuItemFromMaxres("Range", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleRange));
    _toggleRangeButton->setPosition(ccp(bx,220));
    menu->addChild(_toggleRangeButton);
    
    _toggleShotsButton = createMenuItemFromMaxres("Shots", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleShots));
    _toggleShotsButton->setPosition(ccp(bx,190));
    menu->addChild(_toggleShotsButton);
    
    _toggleStatusButton = createMenuItemFromMaxres("Status", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleStatus));
    _toggleStatusButton->setPosition(ccp(bx,160));
    menu->addChild(_toggleStatusButton);
    
    _toggleLockUnitsButton = createMenuItemFromMaxres("", "HelveticaNeue-Bold", 10, ccc3(255,255,255), "LOCK_OF", "LOCK_ON", this, menu_selector(GameInterface::OnToggleLockUnits));
    _toggleLockUnitsButton->setPosition(ccp(bx,300));
    menu->addChild(_toggleLockUnitsButton);
    
    
    
    
    menu->setPosition(CCPoint(0, 0));
    menu->setContentSize(CCSize(100, Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale()));
    menu->setTouchEnabled(true);
    this->addChild(menu);
    
    
    UpdateToggleGridButton();
    UpdateToggleScanButton();
    UpdateToggleRangeButton();
    UpdateToggleShotsButton();
    UpdateToggleStatusButton();
}

#pragma mark - Update left buttons

void GameInterface::UpdateToggleLockUnitsButton()
{
    if(_lockUnits)
        _toggleLockUnitsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("LOCK_ON"));
    else
        _toggleLockUnitsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("LOCK_OF"));
}

void GameInterface::UpdateToggleGridButton()
{
    if(_drawGrid)
        _toggleGridButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleGridButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleScanButton()
{
    if(_drawScan)
        _toggleScanButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleScanButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleRangeButton()
{
    if(_drawRange)
        _toggleRangeButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleRangeButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleShotsButton()
{
    if(_drawShots)
        _toggleShotsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleShotsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleStatusButton()
{
    if(_drawStatus)
        _toggleStatusButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleStatusButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

#pragma mark - Button events

void GameInterface::OnToggleLockUnits()
{
    _lockUnits = !_lockUnits;
    if (!_lockUnits) {
        _lockedUnits.clear();
        MAXStatusRenderer::SharedStatusRenderer()->ClearLockedUnits();
        if (_currentUnit) {
            _lockedUnits.push_back(_currentUnit);
            MAXStatusRenderer::SharedStatusRenderer()->AddUnitToLock(_currentUnit->GetUnitObject());
        }
    }
    UpdateToggleLockUnitsButton();
}

void GameInterface::OnToggleGrid()
{
    _drawGrid = !_drawGrid;
    engine->drawGrid = _drawGrid;
    UpdateToggleGridButton();
}

void GameInterface::OnToggleScan()
{
    _drawScan = !_drawScan;
    MAXStatusRenderer::SharedStatusRenderer()->_drawScan = _drawScan;
    UpdateToggleScanButton();
}

void GameInterface::OnToggleRange()
{
    _drawRange = !_drawRange;
    MAXStatusRenderer::SharedStatusRenderer()->_drawRange  = _drawRange;
    UpdateToggleRangeButton();
}

void GameInterface::OnToggleShots()
{
    _drawShots = !_drawShots;
    MAXStatusRenderer::SharedStatusRenderer()->_drawShots = _drawShots;
    UpdateToggleShotsButton();
}

void GameInterface::OnToggleStatus()
{
    _drawStatus = !_drawStatus;
    MAXStatusRenderer::SharedStatusRenderer()->_drawHealStatus = _drawStatus;
    UpdateToggleStatusButton();
}

#pragma mark - Game events

bool AlreadyExist(GameUnit* object, vector<GameUnit*> *container)
{
    vector<GameUnit*>::iterator obj = find(container->begin(), container->end(), object);
    return (obj != container->end());
}

void GameInterface::RemoveUnitFromLock(GameUnit* object)
{
    bool finded = true;
    while (finded)
    {
        finded = false;
        vector<GameUnit*>::iterator obj = find(_lockedUnits.begin(), _lockedUnits.end(), object);
        if (obj != _lockedUnits.end())
        {
            _lockedUnits.erase(obj);
            finded = true;
        }
    }
    MAXStatusRenderer::SharedStatusRenderer()->RemoveUnitFromLock(object->GetUnitObject());
}

void GameInterface::OnCurrentUnitChanged(GameUnit* unit)
{
    if (unit)
    {
        if (_currentUnit && !_lockUnits)
            RemoveUnitFromLock(_currentUnit);
        
        vector<GameUnit*>::iterator obj = find(_lockedUnits.begin(), _lockedUnits.end(), unit);
        if (obj == _lockedUnits.end())
        {
            _lockedUnits.push_back(unit);
            MAXStatusRenderer::SharedStatusRenderer()->AddUnitToLock(unit->GetUnitObject());
            _currentUnit = unit;
        }
    }
    else
    {
        if (_currentUnit && !_lockUnits)
            RemoveUnitFromLock(_currentUnit);
    
        _currentUnit = unit;
    }
    
}

