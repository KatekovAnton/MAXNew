//
//  GIUnitSelectionMenu.h
//  MAX
//
//  Created by Anton Katekov on 19.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIUnitSelectionMenu__
#define __MAX__GIUnitSelectionMenu__

#include <iostream>
#include "miniPrefix.h"
#include "cocos2d.h"
#include "GIUnitSelectionMenuDelegate.h"

using namespace cocos2d;

class GIUnitSelectionMenu : public CCMenu {
    
    CCMenuItem *CreateMenuItemWithUnit(GameUnit *unit, int index, GameUnit *currentUnit);
    const vector<GameUnit*> _units;
    CCPoint                 _point;
    
public:
    
    GIUnitSelectionMenuDelegate *_delegate_w;
    
    GIUnitSelectionMenu(const vector<GameUnit*> units, GameUnit *currentUnit, const CCPoint &point);
    virtual ~GIUnitSelectionMenu();
    
    void OnButton(CCObject* sender);
};

#endif /* defined(__MAX__GIUnitSelectionMenu__) */
