//
//  ScreenSelectGameTypeDelegate.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenSelectGameTypeDelegate__
#define __MAX__ScreenSelectGameTypeDelegate__

#include <iostream>

class ScreenSelectGameTypeDelegate
{   

public:

	virtual void OnTutorial() = 0;
	virtual void OnHotseat() = 0;
	virtual void OnMultiplayer() = 0;
	virtual void OnEditor() = 0;
	virtual void OnInfo() = 0;
		
};

#endif /* defined(__MAX__ScreenSelectGameTypeDelegate__) */
