//
//  MAXGameControllerDelegate.h
//  MAX
//
//  Created by  Developer on 02.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_MAXGameControllerDelegate_h
#define MAX_MAXGameControllerDelegate_h

#include "miniPrefix.h"

class MAXObjectConfig;

class MAXGameControllerDelegate {

public:
    
    virtual void SelectLargeBuildingConstructionPlaceActionFinished(CCPoint result, MAXObjectConfig *buildingConfig) = 0;
    virtual void SelectSmallBuildingConstructionPathActionFinished(CCPoint result, MAXObjectConfig *buildingConfig) = 0;
    
    virtual void SelectSecondUnitActionFinished(const vector<GameUnit*> units, const CCPoint &cellPoint, UNIT_MENU_ACTION action) = 0;
    
};

#endif
