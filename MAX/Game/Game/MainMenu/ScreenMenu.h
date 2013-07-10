//
//  ScreenMenu.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenMenu__
#define __MAX__ScreenMenu__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class MAXMainMenuController;
class ScreenSelectGameType;

class CCMoveToExtended : public CCMoveTo
{

	CCObject *_endTarget;
	SEL_CallFuncO _endSelector;

public:
	
	void *_parameter;
	bool _flag;

	CCMoveToExtended():_parameter(NULL){};

	bool initWithDuration(float duration, const CCPoint& position, CCObject *endTarget, SEL_CallFuncO endSelector);

    virtual void update(float time);

};

class CCSetFrameExtended : public CCMoveTo
{

	CCObject *_endTarget;
	SEL_CallFuncO _endSelector;

    CCSize m_endSize;
    CCSize m_startSize;
    CCSize m_delta;

public:
	
	void *_parameter;

	CCSetFrameExtended():_parameter(NULL){};

	bool initWithDuration(float duration, const CCSize& endSize, const CCPoint& endPosition, CCObject *endTarget, SEL_CallFuncO endSelector);
	
	virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

};

class CCWaitExtended : public CCActionInterval
{

	CCObject *_endTarget;
	SEL_CallFuncO _endSelector;

public:

	void *_parameter;

	CCWaitExtended():_parameter(NULL){};

	bool initWithDuration(float duration, CCObject *endTarget, SEL_CallFuncO endSelector);

    virtual void update(float time);

};

class ScreenMenu : public CCScene
{   

	bool _freezed;

public:

	MAXMainMenuController	*_menuController;
	
	ccColor4B				_colorBG;
	CCLayerColor			*_layerBg;
	CCLayerColor			*_layerButtonsBG;

	CCArray					*_screenStack;

    ScreenMenu();
    ~ScreenMenu();

	void PushScreen(ScreenMenuElement *screen);
	void PopScreen();
	void PopToFirstScreen();

	void OnMoveDidFinish(CCObject* param);
	void OnPushNewScreen(CCObject* sender);
	void OnPopOldScreen(CCObject* sender);
	void OnFrameChangeDidFinish(CCObject* sender);

	void AlignBGForScreen(ScreenMenuElement *screen, float delayForChangeSize);

	CCPoint CalculatePointsForNode(CCNode *node, int index, int fullNumber, bool outOfScreen, bool down, ScreenMenuElement *screen);
	void MoveNodeAtIndex(CCNode *node, int index, int fullNumber, bool outOfScreen, bool down, ScreenMenuElement *screen);
    
    void InitBaseInterface();

};

#endif /* defined(__MAX__ScreenMenu__) */
