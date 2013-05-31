//
//  GUClearzoneTask.cpp
//  MAX
//
//  Created by  Developer on 01.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GUClearzoneTask.h"

bool GUClearzoneTask::IsFinished()
{
    return _turnsLeft < 1;
}

void GUClearzoneTask::UpdateOnStartTurn()
{
    _turnsLeft++;
}

GUClearzoneTask::GUClearzoneTask(GameUnit *unit)
:GUTask(unit)
{
    
}

GUClearzoneTask::~GUClearzoneTask()
{
    
}