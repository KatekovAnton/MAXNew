//
//  ScreenEditorOptions.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenEditorOptions__
#define __MAX__ScreenEditorOptions__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class ScreenEditorOptions : public ScreenMenuElement
{   

public:

    ScreenEditorOptions();
    virtual ~ScreenEditorOptions();
    
#pragma mark - Button events
    
	void OnNewScenario(CCObject* sender);
	void OnLoadScenario(CCObject* sender);
	void OnBack(CCObject* sender);
	
};

#endif /* defined(__MAX__ScreenEditorOptions__) */
