//
//  ScreenProgressBar.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "ScreenProgressBar.h"
#include "cocos2d.h"
#include "MAXMainMenuController.h"

ScreenProgressBar::ScreenProgressBar()
	:_nodeBase(NULL), _layerFrame(NULL), _layerProgressBar(NULL), _controller(NULL)
{
	
	{
		CCSize size;
		size.width = SingleElementWidth();
		size.height = CCDirector::sharedDirector()->getWinSize().height;
		setContentSize(size);
	}


	if (!_nodeBase)
	{
		_nodeBase = CCNode::create();
		_nodeBase->setContentSize(CCSizeMake(SingleElementWidth(), SingleElementHeight()));
		_nodeBase->setAnchorPoint(ccp(0, 0));
		_nodeBase->setPosition(ccp(0, 0));
		addChild(_nodeBase);
		_buttons->addObject(_nodeBase);
	}


	ccColor4B color;
	
	color = normalColor();
	if (!_layerFrame)
	{
		_layerFrame = CCLayerColor::create(color, SingleElementWidth(), 50);
		_layerFrame->setAnchorPoint(ccp(0, 0));
		_layerFrame->setPosition(ccp(0, 0));
		_nodeBase->addChild(_layerFrame);
	}


	color = selectedColor();
	if (!_layerProgressBar)
	{
		_layerProgressBar = CCLayerColor::create(color, 0, 50);
		_layerProgressBar->setAnchorPoint(ccp(0, 0));
		_layerProgressBar->setPosition(ccp(0, 0));
		_nodeBase->addChild(_layerProgressBar);
	}

	SetProgress(0.0);
}

ScreenProgressBar::~ScreenProgressBar()
{
}

void ScreenProgressBar::SetProgress(float zeroToOne)
{
	if (_layerProgressBar)
	{
		_layerProgressBar->setContentSize(CCSizeMake(SingleElementWidth() * zeroToOne, 50));
	}
	if (_layerFrame)
	{
		_layerFrame->setPosition(ccp(SingleElementWidth() * zeroToOne, 0));
		_layerFrame->setContentSize(CCSizeMake(SingleElementWidth() * (1.0 - zeroToOne), 50));
	}
}

float ScreenProgressBar::SingleElementHeight() const {return 50;}
float ScreenProgressBar::SingleElementDistance() const {return 5;}
float ScreenProgressBar::SingleElementWidth() const {return 320;}

void ScreenProgressBar::ScreenDidAppear()
{
	if (_controller)
	{
		_controller->LoadingScreenDidAppear(this);
	}
}
	
    
