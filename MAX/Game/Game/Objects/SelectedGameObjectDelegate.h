//
//  SelectedGameObjectDelegate.h
//  MAX
//
//  Created by Alexey Malyshko on 4/2/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__SelectedGameObjectDelegate__
#define __MAX__SelectedGameObjectDelegate__

#include <iostream>

class GameUnit;

class SelectedGameObjectDelegate
{
public:
    virtual void onUnitStartMove(GameUnit* unit) = 0;
	virtual void onUnitPauseMove(GameUnit* unit) = 0;
    virtual void onUnitStopMove(GameUnit* unit) = 0;
};

#endif /* defined(__MAX__SelectedGameObjectDelegate__) */
