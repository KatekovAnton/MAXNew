//
//  ScreenMenuElement.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenMenuElement__
#define __MAX__ScreenMenuElement__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "GameInterfacePrefix.h"

using namespace cocos2d;
using namespace std;

class ScreenMenu;

class ScreenMenuElement : public CCNode {

public:

	ScreenMenu	*_navigationMenu;

	CCMenu		*_menu;
	CCArray		*_buttons;

	ScreenMenuElement();
	virtual ~ScreenMenuElement();

	//screen metrics
	virtual float SingleElementHeight() const;
	virtual float SingleElementDistance() const;
	virtual float SingleElementWidth() const;

	//screen events
	virtual void ScreenDidAppear();
	
	CCArray *nodesArray();

	CCNode *CreateSimpleItem(string text, CCObject *target, SEL_MenuHandler selector);
	ccColor4B normalColor() const { ccColor4B normal; normal.a = 150; normal.r = 30; normal.g = 30; normal.b = 30; return normal; };
	ccColor4B selectedColor() const { ccColor4B selected; selected.a = 150; selected.r = 130; selected.g = 130; selected.b = 130; return selected; };

};

#endif /* defined(__MAX__ScreenMenuElement__) */
