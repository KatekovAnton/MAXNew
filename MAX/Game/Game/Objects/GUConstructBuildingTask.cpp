//
//  GUConstructBuildingTask.cpp
//  MAX
//
//  Created by  Developer on 01.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GUConstructBuildingTask.h"


GUConstructBuildingTask::GUConstructBuildingTask(GameUnit *unit, string buildingType, int pathDirectionn, int pathLenth):
GUTask(unit), _type(buildingType), _pathDirection(pathDirectionn), _pathLength(pathLenth)
{
    
}

GUConstructBuildingTask::~GUConstructBuildingTask()
{}

bool GUConstructBuildingTask::IsFinished()
{
    return false;
}

void GUConstructBuildingTask::UpdateOnStartTurn()
{}
