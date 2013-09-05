//
//  GICloseWindow.cpp
//  MAX
//
//  Created by Anton Katekov on 05.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GICloseWindow.h"
#include "GameInterfacePrefix.h"
#include "CocosHelper.h"

GICloseWindow::GICloseWindow()
{
   	setContentSize(ccz(320, 85));

    
	{
        ccColor4B normal; normal.a = 180; normal.r = 10; normal.g = 10; normal.b = 10;
		CCLayerColor* layerN = CCLayerColor::create(normal, 320, 30);
		CCLabelTTF *label = CCLabelTTF::create("Loading. Please stand by.", MAX_DEFAULT_FONT, 10);
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
        layerN->setPosition(ccp(0, 55));
		CocosHelper::MoveNode(layerN->getChildByTag(11), ccp(15, 5));
		addChild(layerN);
	}
    
    
    
	{
        ccColor4B normal; normal.a = 180; normal.r = 10; normal.g = 10; normal.b = 10;
		CCLayerColor *_layerFrame = CCLayerColor::create(normal, 320, 50);
		_layerFrame->setAnchorPoint(ccp(0, 0));
		_layerFrame->setPosition(ccp(0, 0));
		addChild(_layerFrame);
	}

    

    
}

GICloseWindow::~GICloseWindow()
{}
