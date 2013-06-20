//
//  GIUnitSelectionMenuDelegate.h
//  MAX
//
//  Created by Anton Katekov on 19.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GIUnitSelectionMenuDelegate_h
#define MAX_GIUnitSelectionMenuDelegate_h

class GameUnit;

class GIUnitSelectionMenuDelegate {
    
public:
    
    virtual void OnUnitSelected(GameUnit* result, const CCPoint &point) = 0;
    
};

#endif
