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

class GIUnitSelectionMenu : public CCMenu {
    
    CCMenuItem *CreateMenuItemWithUnit(GameUnit *unit);
    const USimpleContainer<GameUnit*> *_units;
    
public:
    
    GIUnitSelectionMenuDelegate *_delegate_w;
    
    GIUnitSelectionMenu(const USimpleContainer<GameUnit*> *units);
    ~GIUnitSelectionMenu();
    
    void OnButton(CCObject* sender);
};

#endif /* defined(__MAX__GIUnitSelectionMenu__) */
