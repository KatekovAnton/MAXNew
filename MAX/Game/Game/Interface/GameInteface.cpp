//
//  GameInteface.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "GameInteface.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "GameUnit.h"
#include "MAXEngine.h"
#include "MAXStatusRenderer.h"
#include "MAXGameController.h"
#include "GameUnitData.h"
#include "GameUnitParameters.h"
#include "GameInterfacePrefix.h"
#include "CCScrollView.h"
#include "GIUnitParametersNode.h"
#include "GIUnitActionMenu.h"
#include "GIUnitSelectionMenu.h"
#include "MAXGame.h"
#include "MAXObjectConfig.h"
#include "StringUtils.h"
#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

float panelW = 70;

bool GameInterface::ShouldReceiveTouch(int x, int y)
{
    float currentPanelW = _panel->getPosition().x + panelW + 12;
    CCRect r = CCRect(0, 0, currentPanelW, this->getContentSize().height);
    bool result = r.containsPoint(CCPoint(x, y));
    
    r = CCRect(getContentSize().width - 212, getContentSize().height - 22, 75,22);
    result |= r.containsPoint(CCPoint(x, y));
    if (GetUnitMenuOpened()) {
        result |= IsTouchInCCNode(x, y, _unitMenu, 10);
    }
    if (GetSelectUnitMenuOpened()) {
        result |= IsTouchInCCNode(x, y, _unitSelectionMenu, 5);
    }
    
    return result;
}

GameInterface::GameInterface()
:_currentUnit(NULL), _unitParameters(NULL), _unitMenu(NULL), _unitSelectionMenu(NULL)
{
    _lockUnits = false;
    
    _drawGrid = true;
    _drawScan = true;
    _drawRange = true;
    _drawShots = false;
    _drawStatus = false;
	_drawResources = false;
    _drawFog = true;
    _drawPathZone = true;
    
    engine->drawGrid = _drawGrid;
    engine->drawPathZone = _drawPathZone;
    if (_currentUnit)
        engine->drawResources = _drawResources || _currentUnit->_unitData->GetIsSurvivor();
    else
        engine->drawResources = _drawResources;
    engine->drawFog = _drawFog;
    
    MAXStatusRenderer::SharedStatusRenderer()->_drawScan = _drawScan;
    MAXStatusRenderer::SharedStatusRenderer()->_drawRange  = _drawRange;
    MAXStatusRenderer::SharedStatusRenderer()->_drawShots = _drawShots;
    MAXStatusRenderer::SharedStatusRenderer()->_drawHealStatus = _drawStatus;
    
}

GameInterface::~GameInterface()
{
	ClearLockedUnits();
}

void GameInterface::InitBaseInterface()
{
    _inited = false;
    setContentSize(CCDirector::sharedDirector()->getVisibleSize());
	//end turn place
//	CCSprite* turnSprite = CCSprite::create("3blocks.png");
//    turnSprite->setAnchorPoint(ccp(0, 1));
//    turnSprite->setPosition(ccp(getContentSize().width - 209, getContentSize().height + 4));
//    addChild(turnSprite);
    
    ccColor4B colorBG; colorBG.a = 200; colorBG.r = 150; colorBG.g = 150; colorBG.b = 150;
    CCLayerColor *turnLayer = CCLayerColor::create(colorBG, 208, 23);
    turnLayer->setPosition(ccp(getContentSize().width - 208, getContentSize().height - 23));
    addChild(turnLayer);
    
    CCLayerColor *turnLayer1 = CCLayerColor::create(CocosHelper::normalColor(), 95, 19);
    turnLayer1->setPosition(ccp(getContentSize().width - 100, getContentSize().height - 19));
    addChild(turnLayer1);
    
    
    
    {
        _labelTurnNumber = CCLabelTTF::create("Turn 1", MAX_DEFAULT_FONT, 10);
        _labelTurnNumber->setTag(BUTTON_LABEL_TAG);
        _labelTurnNumber->setColor(MAX_COLOR_WHITE);
        _labelTurnNumber->setPosition(ccp(getContentSize().width - 60, getContentSize().height - 9));
        _labelTurnNumber->setAnchorPoint(ccp(0, 0.5));
        addChild(_labelTurnNumber);
    }
    {
        _layerPlayerColor = CCLayerColor::create(CocosHelper::normalColor(), 30, 15);
        _layerPlayerColor->setPosition(ccp(getContentSize().width - 98, getContentSize().height - 17));
        addChild(_layerPlayerColor);
    }
    //CCLayerColor *_layerPlayerColor;
    
	//left panel
    _panel = CCNode::create();
    _panel->setPosition(ccp(- panelW + 3, 0));
    _panel->setAnchorPoint(ccp(0, 0));
    _panel->setContentSize(CCSize(panelW, getContentSize().height));
    this->addChild(_panel);
    float bx = 0;
    float bh = 29;
    float bd = 1;
    
//    CCSprite* spr = CCSprite::create("panel.png");
//    spr->setAnchorPoint(ccp(0, 0));
//    spr->setPosition(ccp(panelW - spr->getContentSize().width, 0));
    {
        CCLayerColor *c = CCLayerColor::create(CocosHelper::normalColor(), panelW, getContentSize().height);
        c->setPosition(ccp(0, 0));
        c->setAnchorPoint(ccp(0, 0));
        _panel->addChild(c);
    }
    float top = 280 + 111 + 35;
    float scrolly = 0;
    
    CCSize sz = CCSize(panelW, top);
    if (top >= getContentSize().height)
        sz = CCSize(panelW, getContentSize().height);
    else
        scrolly = getContentSize().height - top;
    CCScrollView* scroll = CCScrollView::create(sz);
    scroll->setAnchorPoint(ccp(0, 0));
    scroll->setPosition(ccp(0, scrolly));
    scroll->setBounceable(true);
    scroll->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
    scroll->setTouchEnabled(true);
    scroll->setClippingToBounds(false);
    scroll->setContentSize(CCSize(panelW, top));
    
    if (top <= getContentSize().height)
        scroll->setTouchEnabled(false);
    else
        scroll->setContentOffset(ccp(0, getContentSize().height - top));
    
    float currentElement = top;
    
    currentElement -= bh + bd;
	_buttonOptions = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Options", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnOptions));
    // createMenuItemFromMaxres("Options", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_OF", "AMMO_ON", this, menu_selector(GameInterface::OnOptions));
    _buttonOptions->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_buttonOptions->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    CCMenu *menu = CCMenu::create(_buttonOptions, nullptr);
	
	currentElement -= 112;
    _unitParameters = GIUnitParametersNode::create();
    _unitParameters->setPosition(ccp(1, currentElement));
    scroll->addChild(_unitParameters);
    
    currentElement -= bh + bd + 2;
    _toggleLockUnitsButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Unit lock", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleLockUnits));
    //createMenuItemFromMaxres("", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "LOCK_OF", "LOCK_ON", this, menu_selector(GameInterface::OnToggleLockUnits));
    _toggleLockUnitsButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleLockUnitsButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
	menu->addChild(_toggleLockUnitsButton);
    
//	{
//		float Scale = CCDirector::sharedDirector()->getContentScaleFactor();
//        CCLabelTTF *label = CCLabelTTF::create("Lock\nunits", MAX_DEFAULT_FONT, 10);
//        label->setColor(MAX_COLOR_WHITE);
//        CCSize sz = label->getContentSize();
//        sz.height /= 2;
//        label->setPosition(ccp(_toggleLockUnitsButton->getContentSize().width + 1, -1));
//		sz.width *= Scale;
//        sz.height *= Scale;
//        label->setContentSize(sz);
//        label->setAnchorPoint(ccp(0, 0));
//        label->setScale(1.0/Scale);
//        _toggleLockUnitsButton->addChild(label);
//    }
    
    currentElement -= bh + bd;
    _toggleGridButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Grid", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleGrid));
    //createMenuItemFromMaxres("Grid", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleGrid));
    _toggleGridButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleGridButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    menu->addChild(_toggleGridButton);
    
    currentElement -= bh + bd;
    _toggleScanButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Scan", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleScan));
    //createMenuItemFromMaxres("Scan", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleScan));
    _toggleScanButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleScanButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    menu->addChild(_toggleScanButton);
    
    currentElement -= bh + bd;
    _toggleRangeButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Range", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleRange));
    //createMenuItemFromMaxres("Range", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleRange));
    _toggleRangeButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleRangeButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    menu->addChild(_toggleRangeButton);
    
    currentElement -= bh + bd;
    _toggleShotsButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Shots", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleShots));
    //createMenuItemFromMaxres("Shots", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleShots));
    _toggleShotsButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleShotsButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    menu->addChild(_toggleShotsButton);
    
    currentElement -= bh + bd;
    _toggleStatusButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Status", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleStatus));
    //createMenuItemFromMaxres("Status", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleStatus));
    _toggleStatusButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleStatusButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    menu->addChild(_toggleStatusButton);
    
    currentElement -= bh + bd;
    _toggleResourcesButton = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Surv", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnToggleResources));
    //createMenuItemFromMaxres("Surv", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleResources));
    _toggleResourcesButton->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_toggleResourcesButton->getChildByTag(BUTTON_LABEL_TAG), ccp(0, 5));
    menu->addChild(_toggleResourcesButton);
    
    currentElement -= bh + bd;
    _togglePathZone = createMenuItemWithLayers(ccz(panelW, bh), CocosHelper::normalColor(), CocosHelper::selectedColor(), "Path zone", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnTogglePathZone));
    //createMenuItemFromMaxres("Path zone", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnTogglePathZone));
    _togglePathZone->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_togglePathZone->getChildByTag(BUTTON_LABEL_TAG), ccp(-6, 5));
    menu->addChild(_togglePathZone);
    
    
    menu->setPosition(CCPoint(0, 0));
    menu->setContentSize(CCSize(100, Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale()));
    menu->setTouchEnabled(true);
    scroll->addChild(menu);
    _panel->addChild(scroll);
    
    CCMenu* menutogglePanel = CCMenu::create();
    menutogglePanel->setPosition(ccp(panelW, getContentSize().height/2.0 - 20));
    menutogglePanel->setAnchorPoint(ccp(0.5, 0.5));
    
    _buttonTogglePanel = createMenuItemFromResources("", "", 10, ccc3(0, 0, 0), "close_btn_bg.png", "close_btn_bg.png", this, menu_selector(GameInterface::OnTogglePanel));
    {
        CCSprite *spr = CCSprite::create("close_btn_close.png");
        spr->setPosition(ccp(_buttonTogglePanel->getContentSize().width/2, _buttonTogglePanel->getContentSize().height/2));
        _buttonTogglePanel->addChild(spr);
    }
    menutogglePanel->setContentSize(_buttonTogglePanel->getContentSize());
    _buttonTogglePanel->setPosition(ccp(_buttonTogglePanel->getContentSize().width/2, _buttonTogglePanel->getContentSize().height/2));
    _buttonTogglePanel->setAnchorPoint(ccp(0.5, 0.5));
    menutogglePanel->addChild(_buttonTogglePanel);
    _panel->addChild(menutogglePanel);
    
//    {
//        ccColor4B normal; normal.a = 255; normal.r = 0; normal.g = 0; normal.b = 0;
//        CCLayerColor *c = CCLayerColor::create(normal, 1, getContentSize().height);
//        c->setPosition(ccp(panelW - 1, 0));
//        c->setAnchorPoint(ccp(0, 0));
//        _panel->addChild(c);
//    }
    
    UpdateToggleGridButton();
    UpdateToggleScanButton();
    UpdateToggleRangeButton();
    UpdateToggleShotsButton();
    UpdateToggleStatusButton();
    UpdateToggleResourcesButton();
    UpdateTogglePathZone();


    _buttonEndTurn = createMenuItemWithLayers(ccz(panelW, 20), CocosHelper::normalColor(), CocosHelper::selectedColor(), "END TURN", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(GameInterface::OnEndTurn));
    //createMenuItemFromMaxres("END TURN", MAX_DEFAULT_FONT, 10, MAX_COLOR_BLACK, "ENDTRN_U", "B_ENDT_D", this, menu_selector(GameInterface::OnEndTurn));
    CocosHelper::MoveNode(_buttonEndTurn->getChildByTag(BUTTON_LABEL_TAG), ccp(-6, 1));
    _buttonEndTurn->setPosition(ccp(0, 0));
    
    CCMenu *menuTurn = CCMenu::create(_buttonEndTurn, nullptr);
    menuTurn->setPosition(getContentSize().width - 203, getContentSize().height - 19);
    menuTurn->setContentSize(CCSize(100, 23));
    menuTurn->setTouchEnabled(true);
    addChild(menuTurn);
    
    this->OnTogglePanel(_buttonTogglePanel);
    _inited = true;
}

void GameInterface::SetNewTurnData(int turnNumber, Color playerColor)
{
    string str = "Turn ";
    str = str + intToString(turnNumber);
    _labelTurnNumber->setString(str.c_str());
    ccColor4B color;
    color.a = 255;
    color.b = playerColor.b;
    color.r = playerColor.r;
    color.g = playerColor.g;
    _layerPlayerColor->initWithColor(color, _layerPlayerColor->getContentSize().width, _layerPlayerColor->getContentSize().height);
}

void GameInterface::ShowUnitSpottedMessage(GameUnit* unit)
{
    SoundEngine::sharedInstance()->PlaySystemSound(SOUND_TYPE_ENEMY_DETECTED);
}

void GameInterface::ClearLockedUnits()
{
    _lockedUnits.clear();
    MAXStatusRenderer::SharedStatusRenderer()->ClearLockedUnits();
}

void GameInterface::SetLockedUnits(vector<GameUnit*> units)
{
    ClearLockedUnits();
    _lockedUnits = units;
    for (int i = 0; i < _lockedUnits.size(); i++) {
        MAXStatusRenderer::SharedStatusRenderer()->AddUnitToLock(_lockedUnits[i]->GetUnitObject());
    }
}

#pragma mark - Update left buttons

void GameInterface::UpdateToggleLockUnitsButton()
{
    _toggleLockUnitsButton->SetSelected(_lockUnits);
//    if(_lockUnits)
//        _toggleLockUnitsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("LOCK_ON"));
//    else
//        _toggleLockUnitsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("LOCK_OF"));
}

void GameInterface::UpdateToggleGridButton()
{
//    if(_drawGrid)
//        _toggleGridButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _toggleGridButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
    _toggleGridButton->SetSelected(_drawGrid);
    engine->drawGrid = _drawGrid;
}

void GameInterface::UpdateToggleScanButton()
{
    _toggleScanButton->SetSelected(_drawScan);
//    if(_drawScan)
//        _toggleScanButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _toggleScanButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleRangeButton()
{
    _toggleRangeButton->SetSelected(_drawRange);
//    if(_drawRange)
//        _toggleRangeButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _toggleRangeButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleShotsButton()
{
    _toggleShotsButton->SetSelected(_drawShots);
//    if(_drawShots)
//        _toggleShotsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _toggleShotsButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleStatusButton()
{
    _toggleStatusButton->SetSelected(_drawStatus);
//    if(_drawStatus)
//        _toggleStatusButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _toggleStatusButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleResourcesButton()
{
    _toggleResourcesButton->SetSelected(_drawResources);
//    if(_drawResources)
//        _toggleResourcesButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _toggleResourcesButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateTogglePathZone()
{
    _togglePathZone->SetSelected(_drawPathZone);
//    if(_drawPathZone)
//        _togglePathZone->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
//    else
//        _togglePathZone->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
    engine->drawPathZone = _drawPathZone;
}

#pragma mark - Button events

void GameInterface::OnOptions(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_gameController->EndMatch();
}

void GameInterface::OnToggleLockUnits(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _lockUnits = !_lockUnits;
    if (!_lockUnits)
    {
        _lockedUnits.clear();
        MAXStatusRenderer::SharedStatusRenderer()->ClearLockedUnits();
        if (_currentUnit)
        {
            _lockedUnits.push_back(_currentUnit);
            MAXStatusRenderer::SharedStatusRenderer()->AddUnitToLock(_currentUnit->GetUnitObject());
        }
    }
    UpdateToggleLockUnitsButton();
}

void GameInterface::OnToggleGrid(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawGrid = !_drawGrid;
    UpdateToggleGridButton();
}

void GameInterface::OnToggleScan(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawScan = !_drawScan;
    MAXStatusRenderer::SharedStatusRenderer()->_drawScan = _drawScan;
    UpdateToggleScanButton();
}

void GameInterface::OnToggleRange(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawRange = !_drawRange;
    MAXStatusRenderer::SharedStatusRenderer()->_drawRange  = _drawRange;
    UpdateToggleRangeButton();
}

void GameInterface::OnToggleShots(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawShots = !_drawShots;
    MAXStatusRenderer::SharedStatusRenderer()->_drawShots = _drawShots;
    UpdateToggleShotsButton();
}

void GameInterface::OnToggleStatus(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawStatus = !_drawStatus;
    MAXStatusRenderer::SharedStatusRenderer()->_drawHealStatus = _drawStatus;
    UpdateToggleStatusButton();
}

void GameInterface::OnToggleResources(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawResources = !_drawResources;
    if (_currentUnit) 
        engine->drawResources = _drawResources || _currentUnit->_unitData->GetIsSurvivor();
    else
        engine->drawResources = _drawResources;
    UpdateToggleResourcesButton();
}

void GameInterface::OnTogglePathZone(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawPathZone = !_drawPathZone;
    engine->drawPathZone = _drawPathZone;
    UpdateTogglePathZone();
}


void GameInterface::OnTogglePanel(CCMenuItem* sender)
{
    if(_inited)
        SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    const static int moveAnimationTag = 0;
    CCAction* currentAction = _panel->getActionByTag(moveAnimationTag);
    if (currentAction)
        return;
    
    CCPoint location = _panel->getPosition();
    bool willMoveToRight = location.x <= -0.5;
    float originX = willMoveToRight ? 0.0 : - panelW;
    CCMoveTo* move = CCMoveTo::create(interfaceAnimationTime, ccp(originX, 0));
    move->setTag(moveAnimationTag);
    CCEaseInOut* action = CCEaseInOut::create(move, 2.0);
    action->setTag(moveAnimationTag);
    _panel->runAction(action);
    
    
//    CCSprite* newSimple = NULL;
//    CCSprite* newSelected = NULL;
//    if (willMoveToRight)
//    {
//        newSimple = CCSprite::create("left_button.png");
//        newSelected = CCSprite::create("left_button_pressed.png");
//    }
//    else
//    {
//        newSimple = CCSprite::create("right_button.png");
//        newSelected = CCSprite::create("right_button_pressed.png");
//    }
//    _buttonTogglePanel->setNormalImage(newSimple);
//    _buttonTogglePanel->setSelectedImage(newSelected);
}

CCPoint GameInterface::GetCenter()
{
    return ccp(getContentSize().width/2, getContentSize().height/2);
}

void GameInterface::SetCurrentTurnNumber(int number)
{}

void GameInterface::SetCurrentPlayerColor(Color color)
{}

void GameInterface::OnEndTurn(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    
	if (_gameController->EndTurn())
        SOUND->PlaySystemSound(SOUND_TYPE_START_OF_TURN);
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

void GameInterface::OnCurrentUnitChanged(GameUnit* unit, bool removeFromLock)
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
        if ((_currentUnit && _lockUnits && removeFromLock) || (_currentUnit && !_lockUnits))
            RemoveUnitFromLock(_currentUnit);
        HideUnitMenu();
    
    }
    _currentUnit = unit;
    
    
    if (_currentUnit)
        engine->drawResources = _drawResources || _currentUnit->_unitData->GetIsSurvivor();
    else
        engine->drawResources = _drawResources;
    
    _unitParameters->SetUnit(_currentUnit);
}

void GameInterface::OnCurrentUnitDataChanged(GameUnit* unit)
{
    if (unit == _currentUnit)
    {
        _unitParameters->UpdateParameters();
        _unitParameters->SetUnit(unit); // TBD: only update data
    }
    else
    {
        _unitParameters->SetUnit(unit);
    }
}

#pragma mark - Unit menu

void GameInterface::ShowMenuForCurrentUni(GIUnitActionMenuDelegate *delegate)
{
    if (!_currentUnit) {
        return;
    }
    if (_unitMenu) {
        return;
    }
    
    vector<UNIT_MENU_ACTION> actions = _currentUnit->GetActionList();
    _unitMenu = new GIUnitActionMenu(actions);
    _unitMenu->setScale(1);
    _unitMenu->_delegate_w = delegate;
    
    UpdateUnitMenuPosition();
    addChild(_unitMenu);
}

void GameInterface::HideUnitMenu()
{
    if (!_unitMenu) 
        return;
    
    _unitMenu->removeFromParentAndCleanup(true);
    _unitMenu->release();
    _unitMenu = NULL;
}

void GameInterface::UpdateUnitMenuPosition()
{
    if (!_unitMenu) {
        return;
    }
    CCPoint point = _currentUnit->GetUnitCell();
    SetNodeNearCell(_unitMenu, point, _currentUnit->GetConfig()->_bSize);
}

#pragma mark - Agreed target

void GameInterface::ShowAgreedTarget(GameUnit *agressor, GameUnit *target, const CCPoint &cellPoint)
{}

void GameInterface::HideAgreedTarget()
{}

void GameInterface::UpdateAgreedTargetPosition(const CCPoint &cellPoint)
{}

#pragma mark - Unit selection menu

void GameInterface::ShowUnitSelectionMenu(GIUnitSelectionMenuDelegate *delegate, const vector<GameUnit*> units, const CCPoint &cellPoint)
{
    if (_unitSelectionMenu) 
        return;
    
    if (_unitMenu) 
        return;
    
    
    _unitSelectionMenu = new GIUnitSelectionMenu(units, _currentUnit, cellPoint);
    _unitSelectionMenu->_delegate_w = delegate;
    
    UpdateUnitSelectionMenuPosition(cellPoint);
    addChild(_unitSelectionMenu);
}

void GameInterface::HideUnitSelectionMenu()
{
    if (!_unitSelectionMenu) 
        return;
    
    _unitSelectionMenu->removeFromParentAndCleanup(true);
    _unitSelectionMenu->release();
    _unitSelectionMenu = NULL;
}

void GameInterface::UpdateUnitSelectionMenuPosition(const CCPoint &cellPoint)
{
    if (!_unitSelectionMenu) 
        return;
    
    SetNodeNearCell(_unitSelectionMenu, cellPoint, 1);
}

void GameInterface::SetNodeNearCell(CCNode *node, const CCPoint &cellPoint, int size)
{
    CCPoint point = cellPoint;
    CCPoint point1 = point;
    point.x *= 64;
    point.y *= 64;
    if (size < 2)
    {
        point.x += 64;
        point.y += 64;
    }
    else
    {
        point.x += 128;
        point.y += 128;
    }
    
    
    point1.x *= 64;
    point1.y *= 64;
    
    point = engine->WorldCoordinatesToScreenCocos(point);
    point1 = engine->WorldCoordinatesToScreenCocos(point1);
    float sideH = point1.y - point.y;
    float menuH = node->getContentSize().height;
    
    float bottomM = point.y + sideH/2 - menuH/2;
    point.y = bottomM;
    
    float scale = node->getScale() - 1.0;
    point.x += node->getContentSize().width * scale * 0.5;
    point.x = (int)point.x;
    point.y = (int)point.y;
    node->setPosition(point);
}


