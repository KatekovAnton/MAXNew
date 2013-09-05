//
//  GameObjectDelegate.h
//  MAX
//
//  Created by Alexey Malyshko on 4/2/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameObjectDelegate__
#define __MAX__GameObjectDelegate__

#include <iostream>

class GameUnit;

class GameObjectDelegate
{
public:
    virtual void onUnitMoveStart(GameUnit* unit) = 0;
	virtual void onUnitMovePause(GameUnit* unit) = 0;
	virtual void onUnitMoveStepBegin(GameUnit* unit) = 0;
	virtual void onUnitMoveStepEnd(GameUnit* unit) = 0;
    virtual void onUnitMoveStop(GameUnit* unit) = 0;
    virtual void onUnitFireStart(GameUnit* unit) = 0;
    virtual void onUnitFireStop(GameUnit* unit) = 0;
	
	virtual void onUnidHided(GameUnit* unit) = 0;
    virtual void onUnitDestroyed(GameUnit* unit) = 0;
    
    virtual bool IsUnitSelected(GameUnit* unit) = 0;
};

#endif /* defined(__MAX__GameObjectDelegate__) */
