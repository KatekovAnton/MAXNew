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

NodeHieraclyOpacity::NodeHieraclyOpacity(CCNode *node)
{
    CCArray* nodestack = CCArray::create();
    nodestack->addObject(node);
    while (nodestack->count() > 0) {
        CCNode *parent = reinterpret_cast<CCNode*>(nodestack->lastObject());
     
        ProcessNode(parent);
        nodestack->removeLastObject();
        if (parent->getChildren())
            nodestack->addObjectsFromArray(parent->getChildren());
        
    }
}

void NodeHieraclyOpacity::ProcessNode(CCNode* node)
{
    CCRGBAProtocol *rgbaPart = dynamic_cast<CCRGBAProtocol*>(node);
    if (rgbaPart)
    {
        baseOpacityData.insert(std::pair<CCNode*, float>(node, rgbaPart->getOpacity()));
        allNodes.push_back(node);
    }
}

bool NodeHieraclyOpacity::NodeHaveOpacity(CCNode *node)
{
    return baseOpacityData.count(node) > 0;
}

float NodeHieraclyOpacity::OpacityForNode(CCNode *node)
{
    if (!NodeHaveOpacity(node))
        return -1;
    return baseOpacityData[node];
}

void NodeHieraclyOpacity::AnimateOpacityToZero()
{
    for (int i = 0; i < allNodes.size(); i++) {
        CCNode *node = allNodes[i];
        CCSetFrameExtended *anim = new CCSetFrameExtended();
       // float opacity = baseOpacityData[node];
        anim->initWithDuration(interfaceAnimationTime, node->getContentSize(), node->getPosition(), 0, NULL, NULL);
        anim->autorelease();
        anim->setTag(0);
        CCEaseInOut* action = CCEaseInOut::create(anim, 3.0);
        action->setTag(0);
        node->runAction(action);
    }
}

void NodeHieraclyOpacity::AnimateOpacityToStartValues()
{
    for (int i = 0; i < allNodes.size(); i++) {
        CCNode *node = allNodes[i];
        CCSetFrameExtended *anim = new CCSetFrameExtended();
        float opacity = baseOpacityData[node];
        anim->initWithDuration(interfaceAnimationTime, node->getContentSize(), node->getPosition(), opacity, NULL, NULL);
        anim->autorelease();
        anim->setTag(0);
        CCEaseInOut* action = CCEaseInOut::create(anim, 3.0);
        action->setTag(0);
        node->runAction(action);
    }
}

void NodeHieraclyOpacity::AnimateOpacityToStartValuesWithDelay(float delay)
{
    for (int i = 0; i < allNodes.size(); i++) {
        CCNode *node = allNodes[i];
        CCSetFrameExtended *anim = new CCSetFrameExtended();
        CCWaitExtended *wait = new CCWaitExtended();
        wait->initWithDuration(delay, NULL, NULL);
        wait->autorelease();
        
        float opacity = baseOpacityData[node];
        anim->initWithDuration(interfaceAnimationTime, node->getContentSize(), node->getPosition(), opacity, NULL, NULL);
        anim->autorelease();
        anim->setTag(0);
        CCEaseInOut* action = CCEaseInOut::create(anim, 3.0);
        action->setTag(0);
        CCAction *seq = CCSequence::createWithTwoActions(wait, action);
        node->runAction(seq);
    }
}

void NodeHieraclyOpacity::SetOpacityToZero()
{
    for (int i = 0; i < allNodes.size(); i++) {
        CCNode *node = allNodes[i];
        CCRGBAProtocol *rgbaPart = dynamic_cast<CCRGBAProtocol*>(node);
        rgbaPart->setOpacity(0);
    }
}

void NodeHieraclyOpacity::SetOpacityToStartValues()
{
    for (int i = 0; i < allNodes.size(); i++) {
        CCNode *node = allNodes[i];
        GLubyte opacity = baseOpacityData[node];
        CCRGBAProtocol *rgbaPart = dynamic_cast<CCRGBAProtocol*>(node);
        rgbaPart->setOpacity(opacity);
    }
}




CCParallaxNodeCustom::CCParallaxNodeCustom()
:_background(NULL)
{
    
}

CCParallaxNodeCustom::~CCParallaxNodeCustom()
{}

void CCParallaxNodeCustom::addChild(CCNode * child, float displacement)
{
    cocos2d::CCNode::addChild(child);
    _nodeDisplacement.insert(pair<CCNode*, float>(child, displacement));
    _nodeBasePositions.insert(pair<CCNode*, CCPoint>(child, child->getPosition()));
}

void CCParallaxNodeCustom::addBackgroundChild(CCNode * child, float displacement)
{
    addChild(child, displacement);
    _background = child;
}

void CCParallaxNodeCustom::removeChild(CCNode* child, bool cleanup)
{
    cocos2d::CCNode::removeChild(child, cleanup);
    _nodeDisplacement.erase(child);
    _nodeBasePositions.erase(child);
}

void CCParallaxNodeCustom::PrepareToParallax()
{
    if (!getChildren())
        return;
    
    for (int i = 0; i < getChildren()->count(); i++) {
        CCNode* node = (CCNode*)getChildren()->objectAtIndex(i);
        _nodeBasePositions.erase(node);
        _nodeBasePositions.insert(pair<CCNode*, CCPoint>(node, node->getPosition()));
    }
}

void CCParallaxNodeCustom::SetDisplacement(CCPoint displacement)
{
    if (!getChildren())
        return;
    
    for (int i = 0; i < getChildren()->count(); i++) {
        CCNode* node = (CCNode*)getChildren()->objectAtIndex(i);
        CCPoint basePos = _nodeBasePositions[node];
        float disp = _nodeDisplacement[node];
        CCPoint pos = ccp(basePos.x + disp * displacement.x, basePos.y + disp * displacement.y);
        node->setPosition(pos);
        
        
    }
}



//move to extended action
bool CCMoveToExtended::initWithDuration(float duration, const CCPoint& position, CCObject *endTarget, SEL_CallFuncO endSelector)
{
	_endTarget = endTarget;
	_endSelector = endSelector;
	return this->CCMoveTo::initWithDuration(duration, position);
}

void CCMoveToExtended::update(float time)
{
	this->CCMoveTo::update(time);
	if(time >= 0.999)
		if (_endTarget && _endSelector && !_selectorCalled)
        {
			(_endTarget->*_endSelector)(this);
            _selectorCalled = true;
        }
}

//set frame action
bool CCSetFrameExtended::initWithDuration(float duration, const CCSize& endSize, const CCPoint& endPosition, const float endAlpha, CCObject *endTarget, SEL_CallFuncO endSelector)
{
	_endTarget = endTarget;
	_endSelector = endSelector;
	m_endSize = endSize;
    m_endAlpha = endAlpha;
	return this->CCMoveTo::initWithDuration(duration, endPosition);
}

void CCSetFrameExtended::update(float time)
{
	CCMoveTo::update(time);
	if (m_pTarget)
    {
		m_pTarget->setContentSize(CCSizeMake(m_startSize.width + m_delta.width * time,
                                             m_startSize.height + m_delta.height * time));
        if (_changeAlpha)
        {
            CCRGBAProtocol *rgbaPart = dynamic_cast<CCRGBAProtocol*>(m_pTarget);
            rgbaPart->setOpacity(m_startAlpha + m_deltaAlpha * time);
        }
    }
	if(time >= 0.999)
		if (_endTarget && _endSelector && !_selectorCalled)
        {
			(_endTarget->*_endSelector)(this);
            _selectorCalled = true;
        }
}

void CCSetFrameExtended::startWithTarget(CCNode *pTarget)
{
    CCMoveTo::startWithTarget(pTarget);
	m_startSize = pTarget->getContentSize();
	m_delta = CCSizeMake(m_endSize.width - m_startSize.width, m_endSize.height - m_startSize.height);
    
    CCRGBAProtocol *rgbaPart = dynamic_cast<CCRGBAProtocol*>(pTarget);
    _changeAlpha = rgbaPart != NULL;
    if (_changeAlpha) {
        m_startAlpha = rgbaPart->getOpacity();
        m_deltaAlpha = m_endAlpha - m_startAlpha;
    }
}

//wait action
bool CCWaitExtended::initWithDuration(float duration, CCObject *endTarget, SEL_CallFuncO endSelector)
{
	_endTarget = endTarget;
	_endSelector = endSelector;
	return this->CCActionInterval::initWithDuration(duration);
}

void CCWaitExtended::update(float time)
{
	if(time >= 0.999)
		if (_endTarget && _endSelector && !_selectorCalled)
        {
			(_endTarget->*_endSelector)(this);
            _selectorCalled = true;
        }
}

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

