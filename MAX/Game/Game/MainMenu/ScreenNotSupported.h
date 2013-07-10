//
//  ScreenNotSupported.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenNotSupported__
#define __MAX__ScreenNotSupported__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class ScreenNotSupported : public ScreenMenuElement
{   

public:

    ScreenNotSupported();
    virtual ~ScreenNotSupported();
	
	virtual float SingleElementHeight() const;
	virtual float SingleElementDistance() const;
	virtual float SingleElementWidth() const;
    
#pragma mark - Button events

	void OnBack(CCObject *sender);
   
};

#endif /* defined(__MAX__ScreenNotSupported__) */
