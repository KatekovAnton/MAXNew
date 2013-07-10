//
//  ScreenProgressBar.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenProgressBar__
#define __MAX__ScreenProgressBar__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class MAXMainMenuController;

class ScreenProgressBar : public ScreenMenuElement
{

	CCNode			*_nodeBase;
	CCLayerColor *_layerFrame;
	CCLayerColor *_layerProgressBar;

public:  

	MAXMainMenuController *_controller;
    
	ScreenProgressBar();
    virtual ~ScreenProgressBar();

	void SetProgress(float zeroToOne);

	virtual float SingleElementHeight() const;
	virtual float SingleElementDistance() const;
	virtual float SingleElementWidth() const;

	virtual void ScreenDidAppear();

};

#endif /* defined(__MAX__ScreenProgressBar__) */
