//
//  GIUnitActionMenu.h
//  MAX
//
//  Created by  Developer on 07.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIUnitActionMenu__
#define __MAX__GIUnitActionMenu__

#include <iostream>
#include <vector>
#include "miniPrefix.h"
#include "cocos2d.h"
#include "GIUnitActionMenuDelegate.h"

using namespace std;
using namespace cocos2d;

class GIUnitActionMenu : public CCMenu {
    
    
public:
    
    GIUnitActionMenuDelegate* _delegate_w;
    
    GIUnitActionMenu(vector<UNIT_MENU_ACTION> buttons);
    ~GIUnitActionMenu();
    
};


#endif /* defined(__MAX__GIUnitActionMenu__) */
