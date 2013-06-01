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
#include "GameUnitData.h"
#include "GameUnitParameters.h"
#include "GameInterfacePrefix.h"
#include "CCScrollView.h"
#include "GIUnitParametersNode.h"
#include "GIUnitActionMenu.h"
#include "MAXGame.h"
#include "MAXObjectConfig.h"

#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

float Scale = 1.0;
float panelW = 70;

CCMenuItemSprite* createMenuItemFromMaxres(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector)
{
    CCSprite* _onToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(selected);
    CCSprite* _ofToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(normal);
    CCMenuItemSprite* result = CCMenuItemSprite::create(_ofToggleGridSprite, _onToggleGridSprite, target, selector);
    result->setAnchorPoint(ccp(0, 0));
    
    
    CCLabelTTF *label = CCLabelTTF::create(title.c_str(), fontName.c_str(), fontSize);
    label->setTag(BUTTON_LABEL_TAG);
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

CCMenuItemSprite* createMenuItemFromResources(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector)
{
    CCSprite* _onToggleGridSprite = CCSprite::create(selected.c_str());
    CCSprite* _ofToggleGridSprite = CCSprite::create(normal.c_str());
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
    
    
    result->addChild(label, 1);
    
    return result;
}

bool GameInterface::ShouldReceiveTouch(int x, int y)
{
    float currentPanelW = _panel->getPosition().x + panelW + 12;
    CCRect r = CCRect(0, 0, currentPanelW, this->getContentSize().height);
    bool result = r.containsPoint(CCPoint(x, y));
    
    r = CCRect(getContentSize().width - 212, getContentSize().height - 22, 75,22);
    result |= r.containsPoint(CCPoint(x, y));
    if (GetUnitMenuOpened()) {
        
        r = CCRect(_unitMenu->getPosition().x, _unitMenu->getPosition().y, _unitMenu->getContentSize().width, _unitMenu->getContentSize().height);
        
        float scale = _unitMenu->getScale() - 1.0;
        r.origin.x -= _unitMenu->getContentSize().width * scale * 0.5;
        r.origin.y -= _unitMenu->getContentSize().height * scale * 0.5;
    
        r.size.width *= _unitMenu->getScale();
        r.size.height *= _unitMenu->getScale();
        
        const float enlargeValue = 20;
        r.origin.x -= enlargeValue;
        r.origin.y -= enlargeValue;
        r.size.width += enlargeValue * 2;
        r.size.height += enlargeValue * 2;
        
        result |= r.containsPoint(CCPoint(x, y));
    }
    
    return result;
}

GameInterface::GameInterface()
:_currentUnit(NULL), _unitParameters(NULL), _unitMenu(NULL)
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
    
    
    Scale = Display::currentDisplay()->GetDisplayScale();
}

GameInterface::~GameInterface()
{
}

void GameInterface::InitBaseInterface()
{
    
    _inited = false;
    CCDirector::sharedDirector()->setDisplayStats(false);
    
    setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    CCSprite* turnSprite = CCSprite::create("3blocks.png");
    turnSprite->setAnchorPoint(ccp(0, 1));
    turnSprite->setPosition(ccp(getContentSize().width - 209, getContentSize().height + 4));
    addChild(turnSprite);
    
    _panel = CCNode::create();
    _panel->setPosition(ccp(0, 0));
    _panel->setAnchorPoint(ccp(0, 0));
    _panel->setContentSize(CCSize(panelW, getContentSize().height));
    this->addChild(_panel);
    float bx = 7;
    
    CCSprite* spr = CCSprite::create("panel.png");
    spr->setAnchorPoint(ccp(0, 0));
    spr->setPosition(ccp(panelW - spr->getContentSize().width, 0));
    _panel->addChild(spr);
    
    float top = 280 + 111;
    float scrollH = top;//?????;
    
    CCSize sz = CCSize(panelW, scrollH);
    if (scrollH >= getContentSize().height)
        sz = CCSize(panelW, getContentSize().height);
    CCScrollView* scroll = CCScrollView::create(sz);
    scroll->setAnchorPoint(ccp(0, 0));
    scroll->setPosition(ccp(0, 0));
    scroll->setBounceable(true);
    scroll->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
    scroll->setTouchEnabled(true);
    scroll->setClippingToBounds(false);
    scroll->setContentSize(CCSize(panelW, top));
    
    if (scrollH <= getContentSize().height)
        scroll->setTouchEnabled(false);
    else
        scroll->setContentOffset(ccp(0, getContentSize().height - scrollH));
    
    float currentElement = top - 111;
    
    _unitParameters = GIUnitParametersNode::create();
    _unitParameters->setPosition(ccp(1, currentElement));
    scroll->addChild(_unitParameters);
    
    
    currentElement -= 30;
    _toggleLockUnitsButton = createMenuItemFromMaxres("", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "LOCK_OF", "LOCK_ON", this, menu_selector(GameInterface::OnToggleLockUnits));
    _toggleLockUnitsButton->setPosition(ccp(bx,currentElement));
    CCMenu *menu = CCMenu::create(_toggleLockUnitsButton, nullptr);
    {
        CCLabelTTF *label = CCLabelTTF::create("Lock\nunits", MAX_DEFAULT_FONT, 10);
        label->setColor(MAX_COLOR_WHITE);
        CCSize sz = label->getContentSize();
        sz.height /= 2;
        label->setPosition(ccp(_toggleLockUnitsButton->getContentSize().width + 1, -1));
        sz.width *= Scale;
        sz.height *= Scale;
        label->setContentSize(sz);
        label->setAnchorPoint(ccp(0, 0));
        label->setScale(1.0/Scale);
        _toggleLockUnitsButton->addChild(label);
    }
    
    currentElement -= 30;
    _toggleGridButton = createMenuItemFromMaxres("Grid", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleGrid));
    _toggleGridButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleGridButton);
    
    currentElement -= 30;
    _toggleScanButton = createMenuItemFromMaxres("Scan", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleScan));
    _toggleScanButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleScanButton);
    
    currentElement -= 30;
    _toggleRangeButton = createMenuItemFromMaxres("Range", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleRange));
    _toggleRangeButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleRangeButton);
    
    currentElement -= 30;
    _toggleShotsButton = createMenuItemFromMaxres("Shots", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleShots));
    _toggleShotsButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleShotsButton);
    
    currentElement -= 30;
    _toggleStatusButton = createMenuItemFromMaxres("Status", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleStatus));
    _toggleStatusButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleStatusButton);
    
    currentElement -= 30;
    _toggleResourcesButton = createMenuItemFromMaxres("Surv", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleResources));
    _toggleResourcesButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleResourcesButton);
    
    currentElement -= 30;
    _toggleFogButton = createMenuItemFromMaxres("Fog", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnToggleFog));
    _toggleFogButton->setPosition(ccp(bx,currentElement));
    menu->addChild(_toggleFogButton);
    
    currentElement -= 30;
    _togglePathZone = createMenuItemFromMaxres("Path zone", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "AMMO_ON", "AMMO_OF", this, menu_selector(GameInterface::OnTogglePathZone));
    _togglePathZone->setPosition(ccp(bx,currentElement));
    CocosHelper::MoveNode(_togglePathZone->getChildByTag(BUTTON_LABEL_TAG), ccp(-6, 0));
    menu->addChild(_togglePathZone);
    
    
    menu->setPosition(CCPoint(0, 0));
    menu->setContentSize(CCSize(100, Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale()));
    menu->setTouchEnabled(true);
    scroll->addChild(menu);
    _panel->addChild(scroll);
    
    CCMenu* menutogglePanel = CCMenu::create();
    menutogglePanel->setPosition(ccp(panelW - 4, getContentSize().height/2.0 - 20));
    menutogglePanel->setAnchorPoint(ccp(0.5, 0.5));
    
    _buttonTogglePanel = createMenuItemFromResources("", "", 10, ccc3(0, 0, 0), "left_button.png", "left_button_pressed.png", this, menu_selector(GameInterface::OnTogglePanel));
    menutogglePanel->setContentSize(_buttonTogglePanel->getContentSize());
    _buttonTogglePanel->setPosition(ccp(_buttonTogglePanel->getContentSize().width/2, _buttonTogglePanel->getContentSize().height/2));
    _buttonTogglePanel->setAnchorPoint(ccp(0.5, 0.5));
    menutogglePanel->addChild(_buttonTogglePanel);
    _panel->addChild(menutogglePanel);
    
    UpdateToggleGridButton();
    UpdateToggleScanButton();
    UpdateToggleRangeButton();
    UpdateToggleShotsButton();
    UpdateToggleStatusButton();
    UpdateToggleResourcesButton();
    UpdateToggleFogButton();
    UpdateTogglePathZone();


    _inited = true;
    
    _buttonEndTurn = createMenuItemFromMaxres("END TURN", MAX_DEFAULT_FONT, 10, MAX_COLOR_BLACK, "ENDTRN_U", "B_ENDT_D", this, menu_selector(GameInterface::OnEndTurn));
    CocosHelper::MoveNode(_buttonEndTurn->getChildByTag(BUTTON_LABEL_TAG), ccp(-4, 1));
    _buttonEndTurn->setPosition(ccp(0, 0));
    
    CCMenu *menuTurn = CCMenu::create(_buttonEndTurn, nullptr);
    menuTurn->setPosition(getContentSize().width - 203, getContentSize().height - 19);
    menuTurn->setContentSize(CCSize(100, 23));
    menuTurn->setTouchEnabled(true);
    addChild(menuTurn);
    
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
    engine->drawGrid = _drawGrid;
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

void GameInterface::UpdateToggleResourcesButton()
{
    if(_drawResources)
        _toggleResourcesButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleResourcesButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateToggleFogButton()
{
    if(_drawFog)
        _toggleFogButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _toggleFogButton->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
}

void GameInterface::UpdateTogglePathZone()
{
    if(_drawPathZone)
        _togglePathZone->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_ON"));
    else
        _togglePathZone->setNormalImage(MAXSCL->CreateSpriteFromSimpleImage("AMMO_OF"));
    engine->drawPathZone = _drawPathZone;
}

#pragma mark - Button events

void GameInterface::OnToggleLockUnits(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_SMALL);
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

void GameInterface::OnToggleFog(CCMenuItem* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    _drawFog = !_drawFog;
    engine->drawFog = _drawFog;
    UpdateToggleFogButton();
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
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    const static int moveAnimationTag = 0;
    CCAction* currentAction = _panel->getActionByTag(moveAnimationTag);
    if (currentAction)
        return;
    
    CCPoint location = _panel->getPosition();
    bool willMoveToRight = location.x <= -0.5;
    float originX = willMoveToRight ? 0.0 : - panelW + 3;
    CCMoveTo* move = CCMoveTo::create(interfaceAnimationTime, ccp(originX, 0));
    move->setTag(moveAnimationTag);
    CCEaseInOut* action = CCEaseInOut::create(move, 2.0);
    action->setTag(moveAnimationTag);
    _panel->runAction(action);
    
    
    CCSprite* newSimple = NULL;
    CCSprite* newSelected = NULL;
    if (willMoveToRight)
    {
        newSimple = CCSprite::create("left_button.png");
        newSelected = CCSprite::create("left_button_pressed.png");
    }
    else
    {
        newSimple = CCSprite::create("right_button.png");
        newSelected = CCSprite::create("right_button_pressed.png");
    }
    _buttonTogglePanel->setNormalImage(newSimple);
    _buttonTogglePanel->setSelectedImage(newSelected);
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
    
    if (game->EndTurn())
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

void GameInterface::UpdateUnitMenuPosition()
{
    if (!_unitMenu) {
        return;
    }
    CCPoint point = _currentUnit->GetUnitCell();
    CCPoint point1 = point;
    point.x *= 64;
    point.y *= 64;
    if (_currentUnit->_unitData->GetSize() < 2)
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
    float menuH = _unitMenu->getContentSize().height;
    
    float bottomM = point.y + sideH/2 - menuH/2;
    point.y = bottomM;
    
    //point is for 1 scale;
    
    float scale = _unitMenu->getScale() - 1.0;
    point.x += _unitMenu->getContentSize().width * scale * 0.5;
    //point.y += _unitMenu->getContentSize().height * scale * 0.25;
    point.x = (int)point.x;
    point.y = (int)point.y;
    _unitMenu->setPosition(point);
}

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
    if (!_unitMenu) {
        return;
    }
    
    _unitMenu->removeFromParentAndCleanup(true);
    _unitMenu = NULL;

}

void GameInterface::OnMenuItemButton(CCMenuItem* sender)
{
    
}


