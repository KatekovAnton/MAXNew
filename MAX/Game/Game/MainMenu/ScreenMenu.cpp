//
//  ScreenMenu.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "MAXContentLoader.h"
#include "ScreenMenu.h"
#include "MAXMainMenuController.h"

#define BUTTON_LABEL_TAG 11

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
		if(_endTarget && _endSelector)
			(_endTarget->*_endSelector)(this);
}

//set frame action
bool CCSetFrameExtended::initWithDuration(float duration, const CCSize& endSize, const CCPoint& endPosition, CCObject *endTarget, SEL_CallFuncO endSelector)
{
	_endTarget = endTarget;
	_endSelector = endSelector;
	m_endSize = endSize;
	return this->CCMoveTo::initWithDuration(duration, endPosition);
}

void CCSetFrameExtended::update(float time)
{
	CCMoveTo::update(time);
	if (m_pTarget)
    {
		m_pTarget->setContentSize(CCSizeMake(m_startSize.width + m_delta.width * time,
			m_startSize.height + m_delta.height * time));
    }
	if(time >= 0.999)
		if(_endTarget && _endSelector)
			(_endTarget->*_endSelector)(this);
}

void CCSetFrameExtended::startWithTarget(CCNode *pTarget)
{
    CCMoveTo::startWithTarget(pTarget);
	m_startSize = pTarget->getContentSize();
	m_delta = CCSizeMake(m_endSize.width - m_startSize.width, m_endSize.height - m_startSize.height);
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
		if (_endTarget && _endSelector)
			(_endTarget->*_endSelector)(this);
}




ScreenMenu::ScreenMenu()
	:_menuController(NULL), _layerButtonsBG(NULL), _layerBg(NULL), _screenStack(NULL), _freezed(false)
{
}

ScreenMenu::~ScreenMenu()
{	
	if (_screenStack)
		_screenStack->release();
}

void ScreenMenu::InitBaseInterface()
{
//ScreenSelectGameType
//start:
//0.Tutorial
//-Scanario list (tutorials) -> Game
//
//
//1.Hot seat
//a.Hew game -> Scenario list (scenaries) -> Game settings -> Game 
//b.Load game -> Scenario list (saves) -> Game
//
//
//2.Multiplayer
//-Not supported
//
//
//3.Editor
//a.New scenario -> Scenario settings -> Editor
//b.Edit scanario -> Scenario list (scenaries) -> Editor
//
//
//4.Info
//a.Credits
//b.Legal information and copyrights
//c.Privacy policy (for multiplayer)
//
//
	
	if (!_layerBg)
	{
		ccColor4B color;
		color.r = 130;
		color.g = 115;
		color.b = 87;
		color.a = 255;

		_layerBg = CCLayerColor::create(color, getContentSize().width, getContentSize().height);
		_layerBg->setAnchorPoint(ccp(0, 0));
		_layerBg->setPosition(ccp(0, 0));
		addChild(_layerBg);
	}

	if (!_layerButtonsBG)
	{
		ccColor4B backgroundColor; backgroundColor.a = 150; backgroundColor.r = 30; backgroundColor.g = 30; backgroundColor.b = 30;
		_layerButtonsBG = CCLayerColor::create(backgroundColor, 0, getContentSize().height);
		_layerButtonsBG->setAnchorPoint(ccp(0, 0));
		_layerButtonsBG->setPosition(ccp(getContentSize().width - 35, 0));
		addChild(_layerButtonsBG);
	}
	
	_screenStack = CCArray::create();
	_screenStack->retain();
}

void ScreenMenu::PushScreen(ScreenMenuElement *screen)
{
	if (_freezed)
		return;
	_freezed = true;

	screen->_navigationMenu = this;
	bool pushNow = true;
	float delay = 0;

	if (_screenStack->count() > 0)
	{//pop old screen
		ScreenMenuElement* element = reinterpret_cast<ScreenMenuElement*>(_screenStack-> objectAtIndex(_screenStack->count() - 1));
		CCArray* nodes = element->nodesArray();
		for (int i = 0; i < nodes->count(); i++)
		{
			CCNode* node = reinterpret_cast<CCNode*>(nodes->objectAtIndex(i));
			MoveNodeAtIndex(node, i, nodes->count(), true, true, element);
		}

		delay = (nodes->count() - 1.0) * 0.1 + interfaceAnimationTime;
		pushNow = false;
	}

	{//align new screen
		CCArray* nodes = screen->nodesArray();
		float y = getContentSize().height/2 - screen->getContentSize().height/2;
		if (y < 0)
			y = 0;
		float width = screen->getContentSize().width;
		screen->setPosition(ccp(getContentSize().width - 35 - width, y));
		addChild(screen);
		_screenStack->addObject(reinterpret_cast<CCObject*>(screen));
	}

	{//check bg size for new screen
		CCSize currentSize = _layerButtonsBG->getContentSize();
		CCSize newSize = CCSizeMake(screen->SingleElementWidth(), getContentSize().height);
		bool needChangeSize = currentSize.width != newSize.width;
		if (needChangeSize)
		{
			float delayForChangeSize = delay;
			delay += interfaceAnimationTime / 2;
			pushNow = false;

			AlignBGForScreen(screen, delayForChangeSize);
		}
	}

	if (pushNow)
	{
		CCArray* nodes = screen->nodesArray();
		for (int i = 0; i < nodes->count(); i++)
		{
			CCNode* node = reinterpret_cast<CCNode*>(nodes->objectAtIndex(i));
			MoveNodeAtIndex(node, i, nodes->count(), false, true, screen);
		}
	}
	else
	{
		CCWaitExtended *wait = new CCWaitExtended();
		wait->initWithDuration(delay, this,  callfuncO_selector(ScreenMenu::OnPushNewScreen));
		wait->autorelease();
		wait->_parameter = screen;
		runAction(wait);
		
		CCArray* nodes = screen->nodesArray();
		for (int i = 0; i < nodes->count(); i++)
		{
			CCNode* node = reinterpret_cast<CCNode*>(nodes->objectAtIndex(i));
			CalculatePointsForNode(node, i, nodes->count(), false, true, screen);
		}
	}
}

void ScreenMenu::OnFrameChangeDidFinish(CCObject* sender)
{

}

void ScreenMenu::OnPushNewScreen(CCObject* sender)
{
	CCWaitExtended* action = reinterpret_cast<CCWaitExtended*>(sender);
	ScreenMenuElement *screen = reinterpret_cast<ScreenMenuElement*>(action->_parameter);

	CCArray *nodes = screen->nodesArray();
	for (int i = 0; i < nodes->count(); i++)
	{
		CCNode* node = reinterpret_cast<CCNode*>(nodes->objectAtIndex(i));
		MoveNodeAtIndex(node, i, nodes->count(), false, true, screen);
	}
}

void ScreenMenu::OnPopOldScreen(CCObject* sender)
{
	CCWaitExtended* action = reinterpret_cast<CCWaitExtended*>(sender);
	ScreenMenuElement *screen = reinterpret_cast<ScreenMenuElement*>(action->_parameter);

	addChild(screen);
	CCArray *nodes = screen->nodesArray();
	for (int i = 0; i < nodes->count(); i++)
	{
		CCNode* node = reinterpret_cast<CCNode*>(nodes->objectAtIndex(i));
		MoveNodeAtIndex(node, i, nodes->count(), false, false, screen);
	}
}

void ScreenMenu::AlignBGForScreen(ScreenMenuElement *screen, float delayForChangeSize)
{
	CCSize newSize = CCSizeMake(screen->SingleElementWidth(), getContentSize().height);
		
	CCPoint finalPos = ccp(getContentSize().width - 35 - newSize.width, 0);

	CCWaitExtended *wait = new CCWaitExtended();
	wait->initWithDuration(delayForChangeSize, NULL, NULL);
	wait->autorelease();

	CCSetFrameExtended* move = new CCSetFrameExtended();
	move->initWithDuration(interfaceAnimationTime, newSize, finalPos, this, callfuncO_selector(ScreenMenu::OnFrameChangeDidFinish));
	move->autorelease();
	move->setTag(0);
	move->_parameter = screen;
	CCEaseInOut* action = CCEaseInOut::create(move, 3.0);
	action->setTag(0);

	CCAction *seq = CCSequence::createWithTwoActions(wait, action);
	_layerButtonsBG->runAction(seq);

}

void ScreenMenu::PopScreen()
{
	if (_freezed)
		return;
	_freezed = true;

	if (_screenStack->count() <= 1)
		return;
	
	
	int ncount;
	{
		ScreenMenuElement* screen = reinterpret_cast<ScreenMenuElement*>(_screenStack->objectAtIndex(_screenStack->count() - 1));
		CCArray* nodes = screen->nodesArray();
		for (int i = 0; i < nodes->count(); i++)
		{
			CCNode* node = reinterpret_cast<CCNode*>(nodes->objectAtIndex(i));
			MoveNodeAtIndex(node, i, nodes->count(), true, false, screen);
		}
		ncount = nodes->count();
	}
	
	float popDelay = (ncount - 1.0) * 0.1 + interfaceAnimationTime;

	{//check bg size for new screen
		ScreenMenuElement* screen = reinterpret_cast<ScreenMenuElement*>(_screenStack->objectAtIndex(_screenStack->count() - 2));
		CCSize currentSize = _layerButtonsBG->getContentSize();
		CCSize newSize = CCSizeMake(screen->SingleElementWidth(), getContentSize().height);
		bool needChangeSize = currentSize.width != newSize.width;
		if (needChangeSize)
		{
			float delayForChangeSize = popDelay;
			popDelay += interfaceAnimationTime / 2;

			AlignBGForScreen(screen, delayForChangeSize);
		}
	}


	{
		ScreenMenuElement* screen = reinterpret_cast<ScreenMenuElement*>(_screenStack->objectAtIndex(_screenStack->count() - 2));
		CCWaitExtended *wait = new CCWaitExtended();
		wait->initWithDuration(popDelay, this,  callfuncO_selector(ScreenMenu::OnPopOldScreen));
		wait->autorelease();
		wait->_parameter = screen;
		runAction(wait);
	}
}

void ScreenMenu::OnMoveDidFinish(CCObject* param)
{
	CCMoveToExtended* action = reinterpret_cast<CCMoveToExtended*>(param);
	ScreenMenuElement *screen = reinterpret_cast<ScreenMenuElement*>(action->_parameter);
	if (action->getTag() != -1 && action->_flag)
	{
		screen->removeFromParentAndCleanup(true);
		int index = _screenStack->indexOfObject(screen);
		if (index == _screenStack->count() - 1)
			_screenStack->removeObject(screen);
	}
	else if (action->getTag() != -1 && !action->_flag && _freezed)
	{
		_freezed = false;
		screen->ScreenDidAppear();
	}

}

CCPoint ScreenMenu::CalculatePointsForNode(CCNode *node, int index, int fullNumber, bool outOfScreen, bool down, ScreenMenuElement *screen)
{
	CCPoint startPos = node->getPosition();
	CCPoint finalPos = node->getPosition();

	float step = (screen->SingleElementHeight() + screen->SingleElementDistance());

	float currentElementStarty = getContentSize().height / 2.0f + (step * ((float)fullNumber/2.0f - (float)index - 1.0));
	float currentElementEndy = getContentSize().height / 2.0f + (step * ((float)fullNumber/2.0f - (float)index - 1.0));

	if (outOfScreen)
	{
		if (down)
			currentElementEndy -= getContentSize().height;
		else
			currentElementEndy += getContentSize().height;
	}
	else
	{
		if (down)
			currentElementStarty += getContentSize().height;
		else
			currentElementStarty -= getContentSize().height;
	}
	startPos.y = currentElementStarty;
	finalPos.y = currentElementEndy;
	
	node->setPosition(startPos);

	return finalPos;
}

void ScreenMenu::MoveNodeAtIndex(CCNode *node, int index, int fullNumber, bool outOfScreen, bool down, ScreenMenuElement *screen)
{
	CCPoint finalPos = CalculatePointsForNode(node, index, fullNumber, outOfScreen, down, screen);
	
	
	CCWaitExtended *wait = new CCWaitExtended();
	float delay = (fullNumber - 1 - index) * 0.1;
	if (!down)
		delay = index * 0.1;
	wait->initWithDuration(delay, NULL, NULL);
	wait->autorelease();

	CCMoveToExtended* move = new CCMoveToExtended();
	move->initWithDuration(interfaceAnimationTime * 2, finalPos, this, callfuncO_selector(ScreenMenu::OnMoveDidFinish));
	move->autorelease();
	move->setTag(-1);
	if (index == 0 && down && outOfScreen)
		move->setTag(1);
	if (index == 0 && down && !outOfScreen)
		move->setTag(1);
	if (index == 0 && down && outOfScreen)
		move->setTag(1);
	if (index == fullNumber - 1 && !down && !outOfScreen)
		move->setTag(1);
	if (index == fullNumber - 1 && !down && outOfScreen)
		move->setTag(1);


	move->_parameter = screen;
	move->_flag = outOfScreen;
	CCEaseInOut* action = CCEaseInOut::create(move, 3.0);
    action->setTag(index);
	
	CCAction *seq = CCSequence::createWithTwoActions(wait, action);
	node->runAction(seq);
}

