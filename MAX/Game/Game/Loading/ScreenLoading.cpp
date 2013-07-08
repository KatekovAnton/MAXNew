//
//  ScreenLoading.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "ScreenLoading.h"
#include "cocos2d.h"

#define BAR_W 320.0

ScreenLoading::ScreenLoading()
	:_layerBg(NULL), _layerFrame(NULL), _layerProgressBar(NULL)
{
}

ScreenLoading::~ScreenLoading()
{
}

void ScreenLoading::SetProgress(float zeroToOne)
{
	if (_layerProgressBar)
	{
		_layerProgressBar->setContentSize(CCSizeMake(BAR_W * zeroToOne, 50));
	}
}

void ScreenLoading::InitBaseInterface()
{
	ccColor4B color;
	color.r = 130;
	color.g = 115;
	color.b = 87;
	color.a = 255;

	if (!_layerBg)
	{
		_layerBg = CCLayerColor::create(color, getContentSize().width, getContentSize().height);
		_layerBg->setAnchorPoint(ccp(0, 0));
		_layerBg->setPosition(ccp(0, 0));
		addChild(_layerBg);
	}

	
	color.r = 0;
	color.g = 0;
	color.b = 0;
	if (!_layerFrame)
	{
		_layerFrame = CCLayerColor::create(color, BAR_W + 2, 52);
		_layerFrame->setAnchorPoint(ccp(0, 0.5));
		_layerFrame->setPosition(ccp(getContentSize().width / 2 - (BAR_W + 2) / 2, getContentSize().height / 2));
		addChild(_layerFrame);
	}


	color.r = 40;
	color.g = 250;
	color.b = 60;
	if (!_layerProgressBar)
	{
		_layerProgressBar = CCLayerColor::create(color, 0, 50);
		_layerProgressBar->setAnchorPoint(ccp(0, 0.5));
		_layerProgressBar->setPosition(ccp(getContentSize().width / 2 - BAR_W / 2, getContentSize().height / 2 + 1));
		addChild(_layerProgressBar);
	}

}
    
