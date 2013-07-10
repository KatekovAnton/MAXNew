//
//  ScreenMenuElement.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "ScreenMenuElement.h"
#include "CocosHelper.h"

ScreenMenuElement::ScreenMenuElement()
{
	_buttons = CCArray::create();
	_buttons->retain();

	{
		CCSize size;
		size.width = SingleElementWidth();
		size.height = CCDirector::sharedDirector()->getWinSize().height;
		setContentSize(size);
	}

	_menu = CCMenu::create();
	_menu->setContentSize(getContentSize());
	_menu->setPosition(0, 0);
	_menu->setAnchorPoint(ccp(0, 0));
	addChild(_menu);
}

CCNode *ScreenMenuElement::CreateSimpleItem(string text, CCObject *target, SEL_MenuHandler selector)
{
	CCMenuItem *_button = createMenuItemWithLayers(ccz(SingleElementWidth(), SingleElementHeight()), normalColor(), selectedColor(), text, MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, target, selector);
	_button->setPosition(ccp(0,0));
	CocosHelper::MoveNode(_button->getChildByTag(11), ccp(15, 5));
	_menu->addChild(_button);
	_buttons->addObject(_button);
	return _button;
}

ScreenMenuElement::~ScreenMenuElement()
{
	_buttons->autorelease();
}

CCArray *ScreenMenuElement::nodesArray()
{
	return _buttons;
}

float ScreenMenuElement::SingleElementHeight() const {return 30;}
float ScreenMenuElement::SingleElementDistance() const {return 5;}
float ScreenMenuElement::SingleElementWidth() const {return 165;}

void ScreenMenuElement::ScreenDidAppear()
{}
	

