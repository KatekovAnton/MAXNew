//
//  ScreenScenarioList.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenScenarioList__
#define __MAX__ScreenScenarioList__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class ScreenScenarioList : public ScreenMenuElement
{   

public:

    ScreenScenarioList();
    virtual ~ScreenScenarioList();

	virtual float SingleElementHeight() const;
	virtual float SingleElementDistance() const;
	virtual float SingleElementWidth() const;
    
#pragma mark - Button events
    
	void OnElement(CCObject *sender);
	void OnBack(CCObject *sender);
   
};

#endif /* defined(__MAX__ScreenScenarioList__) */
