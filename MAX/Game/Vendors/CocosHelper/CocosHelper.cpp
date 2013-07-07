//
//  CocosHelper.cpp
//  MAX
//
//  Created by  Developer on 24.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "CocosHelper.h"
#include "miniPrefix.h"
#include "Display.h"
#include "cocos2d.h"
#include "MAXContentLoader.h"

using namespace cocos2d;

#define BUTTON_LABEL_TAG 11

CCMenuItemSprite* createMenuItemFromMaxres(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector)
{
	Color transparent;
	transparent.r = 0;
	transparent.g = 0;
	transparent.b = 0;
	CCSprite* _onToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(selected, transparent);
    CCSprite* _ofToggleGridSprite = MAXSCL->CreateSpriteFromSimpleImage(normal, transparent);
    CCMenuItemSprite* result = CCMenuItemSprite::create(_ofToggleGridSprite, _onToggleGridSprite, target, selector);
    result->setAnchorPoint(ccp(0, 0));
    
    
    CCLabelTTF *label = CCLabelTTF::create(title.c_str(), fontName.c_str(), fontSize);
    label->setTag(BUTTON_LABEL_TAG);
    label->setColor(titleColor);
    CCSize sz = result->getContentSize();
    sz.height /= 2;
    label->setPosition(ccp(sz.width * 0.25, sz.height * 0.25));
	sz.width *= CCDirector::sharedDirector()->getContentScaleFactor();
    sz.height *= CCDirector::sharedDirector()->getContentScaleFactor();
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
    sz.width *= CCDirector::sharedDirector()->getContentScaleFactor();
    sz.height *= CCDirector::sharedDirector()->getContentScaleFactor();
    label->setContentSize(sz);
    label->setAnchorPoint(ccp(0, 0));
    
    
    result->addChild(label, 1);
    
    return result;
}

bool IsTouchInCCNode(int x, int y, CCNode *node, const float enlargeValue)
{
    CCRect r = CCRect(node->getPosition().x, node->getPosition().y, node->getContentSize().width, node->getContentSize().height);
    
    float scale = node->getScale() - 1.0;
    r.origin.x -= node->getContentSize().width * scale * 0.5;
    r.origin.y -= node->getContentSize().height * scale * 0.5;
    
    r.size.width *= node->getScale();
    r.size.height *= node->getScale();
    
    r.origin.x -= enlargeValue;
    r.origin.y -= enlargeValue;
    r.size.width += enlargeValue * 2;
    r.size.height += enlargeValue * 2;
    
    return r.containsPoint(CCPoint(x, y));
}

void CocosHelper::ProceedCCNode(CCNode* node)
{
    node->setScale(Display::currentDisplay()->GetDisplayScale());
}

void CocosHelper::ProceedCCNodeBack(CCNode* node)
{
    node->setScale(1.0/Display::currentDisplay()->GetDisplayScale());
}

void CocosHelper::SetPosition(CCNode* node, const CCPoint& point)
{
    node->setPosition(ccp(point.x*node->getScale(), point.y*node->getScale()));
}

void CocosHelper::SetContentSize(CCNode* node, const CCSize& size)
{
    node->setContentSize(CCSize(size.width*node->getScale(), size.height*node->getScale()));
}

CCSize CocosHelper::GetContentSize(CCNode* node)
{
    return CCSize(node->getContentSize().width*node->getScale(), node->getContentSize().height*node->getScale());
}

void CocosHelper::MoveNode(CCNode* node, const CCPoint &vector)
{
    if (!node) 
        return;
    
    CCPoint pos = node->getPosition();
    pos.x += vector.x;
    pos.y += vector.y;
    node->setPosition(pos);
}

