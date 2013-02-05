//
//  UserInterface.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/11/12.
//
//

#include "UserInterface.h"
#include "MAXEngine.h"
#include "menu_nodes/CCMenuItem.h"

UserInterface::UserInterface() : _guiScene(nullptr) {
    _guiNodes = CCArray::createWithCapacity(100);
    _guiNodes->retain();
}

CCScene * UserInterface::GetGUISession() {
    CCScene *scene = nullptr;
    if (_guiScene) {
        return _guiScene;
    }
    do
    {
        scene = CCScene::create();
        
        for (int i = 0; i < _guiNodes->count(); i++) {
            scene->addChild((CCNode *)_guiNodes->objectAtIndex(i));
        }
        
        _guiScene = scene;
        _guiScene->retain();
        
    } while (0);
    
    return scene;
}

void UserInterface::Prepare() {
    CCLabelTTF *label = CCLabelTTF::create("Grid ON/OFF", "HelveticaNeue-Bold", 20);
    CCMenuItemLabel *labelItem = CCMenuItemLabel::create(label, (CCObject *)engine, menu_selector(MAXEngine::switchLight));
    ccColor3B color = {255, 255, 255};
    labelItem->setColor(color);
    labelItem->setPosition(CCPointZero);
    labelItem->setAnchorPoint(CCPointZero);
    CCMenu *menu = CCMenu::create(labelItem, nullptr);
    menu->setPosition(CCPoint(00, 280));
    menu->setTouchEnabled(true);
    _guiNodes->addObject(menu);
//    CCTexture2D* pTexture = new CCTexture2D();
//    pTexture->initWithData(const void *data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const cocos2d::CCSize &contentSize);
//    CCSprite* spr = CCSprite::createWithTexture(pTexture);
}


UserInterface::~UserInterface() {
    _guiScene->release();
    _guiNodes->release();
}