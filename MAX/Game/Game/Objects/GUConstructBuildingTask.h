//
//  GUConstructBuildingTask.h
//  MAX
//
//  Created by  Developer on 01.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GUConstructBuildingTask__
#define __MAX__GUConstructBuildingTask__

#include <iostream>
#include "GUTask.h"
#include "miniPrefix.h"

class GameUnitParameters;

class GUConstructBuildingTask : public GUTask {
    
public:
    
    string _type;
    
    int _pathDirection;
    int _pathLength;
    
    
    virtual bool IsFinished();
    virtual void UpdateOnStartTurn();
    
    
    GUConstructBuildingTask(GameUnit *unit, string buildingType, int pathDirectionn, int pathLenth);
    ~GUConstructBuildingTask();

};

#endif /* defined(__MAX__GUConstructBuildingTask__) */
