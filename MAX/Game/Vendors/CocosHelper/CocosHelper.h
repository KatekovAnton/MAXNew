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

CCMenuItem* createMenuItemWithLayers(CCSize size, ccColor4B normalColor, ccColor4B selectedColor, string title, string fontName, int fontSize, ccColor3B titleColor, CCObject* target, SEL_MenuHandler selector);
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
};

class CCMenuItemNodes : public CCMenuItem {

	CCNode *_nodeNormal;
	CCNode *_nodeSelected;

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
	
	static CCMenuItemNodes* create(CCNode *normal, CCNode *selected, CCObject *rec, SEL_MenuHandler selector);

};

#endif /* defined(__MAX__CocosHelper__) */
