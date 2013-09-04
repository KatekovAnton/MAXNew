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


ScreenSelectGameType::ScreenSelectGameType()
	:_delegete_w(NULL), ScreenMenuElement()
{
	
	float currentElement = getContentSize().height / 2 + (35 * 5) / 2;
	float bx = 0;
	ccColor4B normal = normalColor();
	ccColor4B selected = selectedColor();

	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Tutorial", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenSelectGameType::OnTutorial));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Hot seat", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenSelectGameType::OnHotseat));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Multiplayer", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenSelectGameType::OnMultiplayer));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Editor", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenSelectGameType::OnEditor));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Info", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenSelectGameType::OnInfo));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
}

ScreenSelectGameType::~ScreenSelectGameType()
{
	_buttons->autorelease();
}

#pragma mark - Button events

void ScreenSelectGameType::OnTutorial(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnTutorial();
}

void ScreenSelectGameType::OnHotseat(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnHotseat();
}

void ScreenSelectGameType::OnMultiplayer(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnMultiplayer();
}

void ScreenSelectGameType::OnEditor(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnEditor();
}

void ScreenSelectGameType::OnInfo(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	if (_delegete_w)
		_delegete_w->OnInfo();
}

