//
//  ScreenNotSupported.cpp
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "miniPrefix.h"
#include "ScreenNotSupported.h"
#include "MAXContentLoader.h"
#include "Display.h"
#include "CocosHelper.h"
#include "CCScrollView.h"
#include "MAXGame.h"
#include "ScreenMenu.h"

#include "SoundEngine.h"

using namespace extension;

#define BUTTON_LABEL_TAG 11

ScreenNotSupported::ScreenNotSupported()
{
	{
		CCSize size;
		size.width = SingleElementWidth();
		size.height = CCDirector::sharedDirector()->getWinSize().height;
		setContentSize(size);
	}

	CCLayerColor* layerN = CCLayerColor::create(normalColor(), SingleElementWidth(), SingleElementHeight());
	CCLabelTTF *label = CCLabelTTF::create("Not supported for now.", MAX_DEFAULT_FONT, 10);
    label->setTag(BUTTON_LABEL_TAG);
	label->setColor(MAX_COLOR_WHITE);
    CCSize sz = layerN->getContentSize();
    sz.height /= 2;
    label->setPosition(ccp((int)(sz.width * 0.25), (int)(sz.height * 0.25)));
    sz.width *= CCDirector::sharedDirector()->getContentScaleFactor();
    sz.height *= CCDirector::sharedDirector()->getContentScaleFactor();
    label->setContentSize(sz);
    label->setAnchorPoint(ccp(0, 0));
	layerN->addChild(label);
	CocosHelper::MoveNode(layerN->getChildByTag(11), ccp(15, 5));
	
	addChild(layerN);
	_buttons->addObject(layerN);

	CreateSimpleItem("Back", this, menu_selector(ScreenNotSupported::OnBack));
	
}

ScreenNotSupported::~ScreenNotSupported()
{
}

float ScreenNotSupported::SingleElementHeight() const {return 30;}
float ScreenNotSupported::SingleElementDistance() const {return 5;}
float ScreenNotSupported::SingleElementWidth() const {return 320;}

#pragma mark - Button events

void ScreenNotSupported::OnBack(CCObject *sender)
{
	SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
	_navigationMenu->PopScreen();
}



   
