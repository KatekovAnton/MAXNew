//
//  ScreenScenarioList.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "ScreenScenarioList.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "ScreenMenu.h"
#include "CCScrollView.h"
#include "MAXGame.h"

#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

ScreenScenarioList::ScreenScenarioList()
	:ScreenMenuElement()
{
	{
		CCSize size;
		size.width = SingleElementWidth();
		size.height = CCDirector::sharedDirector()->getWinSize().height;
		setContentSize(size);
	}

	CreateSimpleItem("Scenario 0", this, menu_selector(ScreenScenarioList::OnElement));
	CreateSimpleItem("Scenario 1", this, menu_selector(ScreenScenarioList::OnElement));
	CreateSimpleItem("Scenario 2", this, menu_selector(ScreenScenarioList::OnElement));
	CreateSimpleItem("Scenario 3", this, menu_selector(ScreenScenarioList::OnElement));
	CreateSimpleItem("Back", this, menu_selector(ScreenScenarioList::OnBack));
}

ScreenScenarioList::~ScreenScenarioList()
{
}

#pragma mark - Button events
    
void ScreenScenarioList::OnElement(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
}

void ScreenScenarioList::OnBack(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_navigationMenu->PopScreen();
}

float ScreenScenarioList::SingleElementHeight() const {return 30;}
float ScreenScenarioList::SingleElementDistance() const {return 5;}
float ScreenScenarioList::SingleElementWidth() const {return 320;}


