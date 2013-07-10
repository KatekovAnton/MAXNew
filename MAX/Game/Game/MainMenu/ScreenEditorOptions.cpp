//
//  ScreenEditorOptions.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "ScreenEditorOptions.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"
#include "ScreenMenu.h"
#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

ScreenEditorOptions::ScreenEditorOptions()
	:ScreenMenuElement()
{
	float currentElement = getContentSize().height / 2 + (35 * 5) / 2;
	float bx = 0;
	ccColor4B normal = normalColor();
	ccColor4B selected = selectedColor();

	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "New Scenario", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenEditorOptions::OnNewScenario));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Load Scenario", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenEditorOptions::OnLoadScenario));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
	currentElement -= 35;
	{
		CCMenuItem *_button = createMenuItemWithLayers(ccz(165, 30), normal, selected, "Back", MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, menu_selector(ScreenEditorOptions::OnBack));
		_button->setPosition(ccp(bx,currentElement));
		CocosHelper::MoveNode(_button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
		_menu->addChild(_button);
		_buttons->addObject(_button);
	}
}

ScreenEditorOptions::~ScreenEditorOptions()
{
}

#pragma mark - Button events
    
void ScreenEditorOptions::OnNewScenario(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
}

void ScreenEditorOptions::OnLoadScenario(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
}
	
void ScreenEditorOptions::OnBack(CCObject* sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_navigationMenu->PopScreen();
}
	



