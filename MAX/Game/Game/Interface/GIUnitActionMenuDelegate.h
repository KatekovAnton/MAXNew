//
//  GIUnitActionMenuDelegate.h
//  MAX
//
//  Created by  Developer on 07.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GIUnitActionMenuDelegate_h
#define MAX_GIUnitActionMenuDelegate_h

class GIUnitActionMenuDelegate {
    
public:
    
    virtual void OnUnitMenuItemSelected(UNIT_MENU_ACTION action) = 0;
    virtual bool IsUnitActionSelected(UNIT_MENU_ACTION action) = 0;
    
};

#endif
