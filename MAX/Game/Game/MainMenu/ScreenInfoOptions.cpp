//
//  ScreenInfoOptions.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "ScreenInfoOptions.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"
#include "ScreenMenu.h"

#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

ScreenInfoOptions::ScreenInfoOptions()
{
	
//4.Info
//a.Credits
//b.Legal information and copyrights
//c.Privacy policy (for multiplayer)

	CreateSimpleItem("Credits", this, menu_selector(ScreenInfoOptions::OnCredits));
	CreateSimpleItem("Legal Info", this, menu_selector(ScreenInfoOptions::OnLegal));
	CreateSimpleItem("Privacy Policy", this, menu_selector(ScreenInfoOptions::OnPrivacy));
	CreateSimpleItem("Back", this, menu_selector(ScreenInfoOptions::OnBack));
}

ScreenInfoOptions::~ScreenInfoOptions()
{
}

#pragma mark - Button events
	
void ScreenInfoOptions::OnCredits(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
}

void ScreenInfoOptions::OnLegal(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
}

void ScreenInfoOptions::OnPrivacy(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
}  

void ScreenInfoOptions::OnBack(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_navigationMenu->PopScreen();
}    

