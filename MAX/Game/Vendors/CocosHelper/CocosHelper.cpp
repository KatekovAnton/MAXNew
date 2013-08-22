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

CCMenuItemNodes* createMenuItemWithLayers(CCSize size, ccColor4B normalColor, ccColor4B selectedColor, string title, string fontName, int fontSize, ccColor3B titleColor, CCObject* target, SEL_MenuHandler selector)
{
	CCLayerColor* layerN = CCLayerColor::create(normalColor, size.width, size.height);
	CCLayerColor* layerS = CCLayerColor::create(selectedColor, size.width, size.height);
	CCMenuItemNodes* result = CCMenuItemNodes::create(layerN, layerS, target, selector);
	result->setContentSize(size);
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


    result->addChild(label, 1);
    return result;
}

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
    label->setTag(BUTTON_LABEL_TAG);
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


CCMenuItemNodes::~CCMenuItemNodes()
{
	if (_nodeNormal)
	{
		_nodeNormal->removeFromParentAndCleanup(true);
		_nodeNormal->autorelease();
	}
	if (_nodeSelected)
	{
		_nodeSelected->removeFromParentAndCleanup(true);
		_nodeSelected->autorelease();
	}
}

bool CCMenuItemNodes::initWithTarget(CCNode *normal, CCNode *selected, CCObject *rec, SEL_MenuHandler selector)
{
    _isSelected = false;
	this->CCMenuItem::initWithTarget(rec, selector);
	if (normal)
	{
		_nodeNormal = normal;
		_nodeNormal->retain();
		_nodeNormal->setAnchorPoint(ccp(0, 0));
		_nodeNormal->setPosition(ccp(0, 0));
		addChild(_nodeNormal);
		setContentSize(_nodeNormal->getContentSize());

	}
	if (selected)
	{
		_nodeSelected = selected;
		_nodeSelected->retain();
		_nodeSelected->setAnchorPoint(ccp(0, 0));
		_nodeSelected->setPosition(ccp(0, 0));
		addChild(_nodeSelected);
	}
	updateNodes();
	return true;
}

void CCMenuItemNodes::updateNodes()
{
	if (_nodeNormal)
		_nodeNormal->setVisible((!this->m_bIsSelected) || _isSelected);
	if (_nodeSelected)
		_nodeSelected->setVisible((this->m_bIsSelected) || _isSelected);
}

void CCMenuItemNodes::selected()
{
	this->CCMenuItem::selected();
	updateNodes();
}

void CCMenuItemNodes::unselected()
{
	this->CCMenuItem::unselected();
	updateNodes();
}

void CCMenuItemNodes::SetSelected(bool flag)
{
    _isSelected = flag;
    updateNodes();
}

CCMenuItemNodes* CCMenuItemNodes::create(CCNode *normal, CCNode *selected, CCObject *rec, SEL_MenuHandler selector)
{
	CCMenuItemNodes *resilt = new CCMenuItemNodes();
	resilt->initWithTarget(normal, selected, rec, selector);
	resilt->autorelease();
	return resilt;
}

