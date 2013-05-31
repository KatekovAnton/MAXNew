//
//  GUClearzoneTask.h
//  MAX
//
//  Created by  Developer on 01.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GUClearzoneTask__
#define __MAX__GUClearzoneTask__

#include <iostream>
#include "GUTask.h"
#include "miniPrefix.h"

class GameUnitParameters;

class GUClearzoneTask : public GUTask {
    
public:
    
    virtual bool IsFinished();
    virtual void UpdateOnStartTurn();
    
    
    GUClearzoneTask(GameUnit *unit);
    ~GUClearzoneTask();
    
};

#endif /* defined(__MAX__GUClearzoneTask__) */
