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
    virtual void onUnitMoveStart(GameUnit* unit) = 0;
	virtual void onUnitMovePause(GameUnit* unit) = 0;
	virtual void onUnitMoveStepBegin(GameUnit* unit) = 0;
    virtual void onUnitMoveStop(GameUnit* unit) = 0;
    virtual void onUnitFireStart(GameUnit* unit) = 0;
    virtual void onUnitFireStop(GameUnit* unit) = 0;
};

#endif /* defined(__MAX__SelectedGameObjectDelegate__) */
