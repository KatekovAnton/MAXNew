//
//  GUTask.h
//  MAX
//
//  Created by  Developer on 01.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GUTask__
#define __MAX__GUTask__

#include <iostream>

class GameUnit;

class GUTask {
    
public:
    
    int _turnsLeft;
    
    GameUnit* _unitWorker_w;
    
    virtual bool IsFinished() = 0;
    virtual void UpdateOnStartTurn() = 0;
    
    GUTask(GameUnit* unit);
    ~GUTask();
};

#endif /* defined(__MAX__GUTask__) */
