//
//  ScreenSelectGameType.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "ScreenSelectGameType.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"

#include "SoundEngine.h"
#include "GameInterfacePrefix.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

ScreenSelectGameType::ScreenSelectGameType()
	:_delegete_w(NULL)
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();

	//
	{
		CCSprite* background = MAXSCL->CreateSpriteFromSimpleImage("PANELTOP");
		background->setAnchorPoint(ccp(0, 0));
		background->setPosition(ccp(0, 0));
		addChild(background);
		setContentSize(background->getContentSize());
	}
	CCMenu *menu = CCMenu::create();
	menu->setContentSize(getContentSize());
	menu->setPosition(0, 0);
	menu->setAnchorPoint(ccp(0, 0));
	addChild(menu);

	
	float currentElement = getContentSize().height - 35;
	float bx = 2;
	{
		CCMenuItemSprite *_button = createMenuItemFromMaxres("Tutorial", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "OBAR_UP", "OBAR_DN", this, menu_selector(ScreenSelectGameType::OnTutorial));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 6));
		menu->addChild(_button);
	}
	currentElement -= 35;
	{
		CCMenuItemSprite *_button = createMenuItemFromMaxres("Hot seat", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "OBAR_UP", "OBAR_DN", this, menu_selector(ScreenSelectGameType::OnHotseat));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 6));
		menu->addChild(_button);
	}
	currentElement -= 35;
	{
		CCMenuItemSprite *_button = createMenuItemFromMaxres("Multiplayer", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "OBAR_UP", "OBAR_DN", this, menu_selector(ScreenSelectGameType::OnMultiplayer));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 6));
		menu->addChild(_button);
	}
	currentElement -= 35;
	{
		CCMenuItemSprite *_button = createMenuItemFromMaxres("Editor", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "OBAR_UP", "OBAR_DN", this, menu_selector(ScreenSelectGameType::OnEditor));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 6));
		menu->addChild(_button);
	}
	currentElement -= 35;
	{
		CCMenuItemSprite *_button = createMenuItemFromMaxres("Info", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, "OBAR_UP", "OBAR_DN", this, menu_selector(ScreenSelectGameType::OnInfo));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 6));
		menu->addChild(_button);
	}
	currentElement -= 35;
}

ScreenSelectGameType::~ScreenSelectGameType()
{
}

#pragma mark - Button events

void ScreenSelectGameType::OnTutorial(CCMenuItem* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnTutorial();
}

void ScreenSelectGameType::OnHotseat(CCMenuItem* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnHotseat();
}

void ScreenSelectGameType::OnMultiplayer(CCMenuItem* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnMultiplayer();
}

void ScreenSelectGameType::OnEditor(CCMenuItem* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnEditor();
}

void ScreenSelectGameType::OnInfo(CCMenuItem* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnInfo();
}

