//
//  CocosHelper.h
//  MAX
//
//  Created by  Developer on 24.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__CocosHelper__
#define __MAX__CocosHelper__

#include <iostream>
#include "cocos2d.h"
#include <string>

using namespace std;
using namespace cocos2d;

#define ccz(__X__,__Y__) cocos2d::CCSizeMake((float)(__X__), (float)(__Y__))

class NodeHieraclyOpacity
{
    map<CCNode*, float> baseOpacityData;
    void ProcessNode(CCNode*);
    
public:
    NodeHieraclyOpacity(CCNode *node);
    
    bool NodeHaveOpacity(CCNode *node);
    float OpacityForNode(CCNode *node);
    
};

class CCMoveToExtended : public CCMoveTo
{
    
	CCObject *_endTarget;
	SEL_CallFuncO _endSelector;
    bool _selectorCalled;
    
public:
	
	void *_parameter;
	bool _flag;
    
	CCMoveToExtended():_parameter(NULL), _selectorCalled(false){};
    
	bool initWithDuration(float duration, const CCPoint& position, CCObject *endTarget, SEL_CallFuncO endSelector);
    
    virtual void update(float time);
    
};

class CCSetFrameExtended : public CCMoveTo
{
    
	CCObject *_endTarget;
	SEL_CallFuncO _endSelector;
    bool _selectorCalled;
    
    CCSize m_endSize;
    CCSize m_startSize;
    CCSize m_delta;
    
    bool _changeAlpha;
    float m_endAlpha;
    float m_startAlpha;
    float m_deltaAlpha;
    
public:
	
	void *_parameter;
    
	CCSetFrameExtended():_parameter(NULL), _selectorCalled(false){};
    
	bool initWithDuration(float duration, const CCSize& endSize, const CCPoint& endPosition, const float endAlpha, CCObject *endTarget, SEL_CallFuncO endSelector);
	
	virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    
};

class CCWaitExtended : public CCActionInterval
{
    
	CCObject *_endTarget;
	SEL_CallFuncO _endSelector;
    bool _selectorCalled;
    
public:
    
	void *_parameter;
    
	CCWaitExtended():_parameter(NULL), _selectorCalled(false){};
    
	bool initWithDuration(float duration, CCObject *endTarget, SEL_CallFuncO endSelector);
    
    virtual void update(float time);
    
};

class CCMenuItemNodes;

CCMenuItemNodes* createMenuItemWithLayers(CCSize size, ccColor4B normalColor, ccColor4B selectedColor, string title, string fontName, int fontSize, ccColor3B titleColor, CCObject* target, SEL_MenuHandler selector);
CCMenuItemSprite* createMenuItemFromMaxres(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector);
CCMenuItemSprite* createMenuItemFromResources(string title, string fontName, int fontSize, ccColor3B titleColor, string normal, string selected, CCObject* target, SEL_MenuHandler selector);
bool IsTouchInCCNode(int x, int y, CCNode *node, const float enlargeValue);


class CocosHelper {
    
public:
    
    static void ProceedCCNode(CCNode* node);
    static void ProceedCCNodeBack(CCNode* node);

    static void SetPosition(CCNode* node, const CCPoint& point);
    static void SetContentSize(CCNode* node, const CCSize& size);
    
    static void MoveNode(CCNode* node, const CCPoint &vector);
    
    static CCSize GetContentSize(CCNode* node);
    
    static ccColor4B normalColor() { ccColor4B normal; normal.a = 190; normal.r = 30; normal.g = 30; normal.b = 30; return normal; };
	static ccColor4B selectedColor() { ccColor4B selected; selected.a = 150; selected.r = 130; selected.g = 130; selected.b = 130; return selected; };

};

class CCMenuItemNodes : public CCMenuItem {

	CCNode *_nodeNormal;
	CCNode *_nodeSelected;
    
    bool _isSelected;

public:
	
    CCMenuItemNodes()
		:_nodeNormal(NULL),
		_nodeSelected(NULL)
    {}
    virtual ~CCMenuItemNodes();
	
    bool initWithTarget(CCNode *normal, CCNode *selected, CCObject *rec, SEL_MenuHandler selector);
	void updateNodes();

	virtual void selected();
    virtual void unselected();
    
    void SetSelected(bool flag);
	
	static CCMenuItemNodes* create(CCNode *normal, CCNode *selected, CCObject *rec, SEL_MenuHandler selector);
    
    

};

#endif /* defined(__MAX__CocosHelper__) */
