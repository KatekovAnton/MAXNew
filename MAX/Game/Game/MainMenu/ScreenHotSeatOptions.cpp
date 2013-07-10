//
//  ScreenHotSeatOptions.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "ScreenHotSeatOptions.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"
#include "ScreenMenu.h"
#include "MAXMainMenuController.h"
#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

ScreenHotSeatOptions::ScreenHotSeatOptions()
	:_menuController(NULL)
{
	CreateSimpleItem("Start test match", this, menu_selector(ScreenHotSeatOptions::OnTestMatch));
	CreateSimpleItem("Back", this, menu_selector(ScreenHotSeatOptions::OnBack));
}

ScreenHotSeatOptions::~ScreenHotSeatOptions()
{
}
   
#pragma mark - Button events

void ScreenHotSeatOptions::OnTestMatch(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_menuController->StartMatch();
}

void ScreenHotSeatOptions::OnBack(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_navigationMenu->PopScreen();
}