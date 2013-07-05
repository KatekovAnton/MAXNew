//
//  MAXGameInputControllerDelegate.h
//  MAX
//
//  Created by  Developer on 02.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_MAXGameInputControllerDelegate_h
#define MAX_MAXGameInputControllerDelegate_h

#include "miniPrefix.h"

class MAXObjectConfig;
class GameMatch;

class MAXGameInputControllerDelegate {

public:
    
    virtual void SelectLargeBuildingConstructionPlaceActionFinished(CCPoint result, MAXObjectConfig *buildingConfig) = 0;
    virtual void SelectSmallBuildingConstructionPathActionFinished(CCPoint result, MAXObjectConfig *buildingConfig) = 0;
    virtual void SelectSecondUnitActionFinished(const vector<GameUnit*> units, const CCPoint &cellPoint, UNIT_MENU_ACTION action) = 0;
    virtual void SelectSecondUnitActionCanceled() = 0;
	virtual GameMatch *GetCurrentMatch() = 0;    
};

#endif
