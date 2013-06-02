//
//  GUConstructBuildingTask.cpp
//  MAX
//
//  Created by  Developer on 01.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GUConstructBuildingTask.h"
#include "GameUnit.h"
#include "GameMatchPlayer.h"

GUConstructBuildingTask::GUConstructBuildingTask(GameUnit *unit, string buildingType, int pathDirectionn, int pathLenth, CCPoint cell):
GUTask(unit), _type(buildingType), _pathDirection(pathDirectionn), _pathLength(pathLenth), _cell(cell), _constructionUnit(NULL)
{

}

GUConstructBuildingTask::~GUConstructBuildingTask()
{
    AbortTask();
}

bool GUConstructBuildingTask::IsFinished()
{
    return false;
}

void GUConstructBuildingTask::UpdateOnStartTurn()
{}

void GUConstructBuildingTask::AbortTask()
{
    if (_constructionUnit)
    {
        _constructionUnit->Destroy();
        _constructionUnit = NULL;
    }
}

void GUConstructBuildingTask::StartTask()
{
    _constructionUnit = _unitWorker_w->_owner_w->CreateUnit(_cell.x, _cell.y, _type, 0);
    _constructionUnit->BeginConstructionSequence();
    _constructionUnit->PlaceUnitOnMap();
}

void GUConstructBuildingTask::FinishTask()
{
    _constructionUnit->EndConstructionSequense();
    _constructionUnit = NULL;
}


