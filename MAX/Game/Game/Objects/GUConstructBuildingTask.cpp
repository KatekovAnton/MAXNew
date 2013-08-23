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
GUTask(unit), _type(buildingType), _pathDirection(pathDirectionn), _pathLength(pathLenth), _cell(cell), _constructionUnit_w(NULL), _turns(0)
{

}

GUConstructBuildingTask::~GUConstructBuildingTask()
{
    AbortTask();
}

bool GUConstructBuildingTask::IsFinished()
{
    return _turns >=2;
}

void GUConstructBuildingTask::UpdateOnStartTurn()
{
    if (!IsFinished())
        _turns ++;
}

void GUConstructBuildingTask::AbortTask()
{
    if (_constructionUnit_w)
    {
        _constructionUnit_w->Destroy();
        _constructionUnit_w->RemoveUnitFromMap();
        delete _constructionUnit_w;
        _constructionUnit_w = NULL;
    }
}

void GUConstructBuildingTask::StartTask()
{
    _constructionUnit_w = _unitWorker_w->_owner_w->CreateUnit(_cell.x, _cell.y, _type, 0);
	_constructionUnit_w->BeginConstructionSequence(_unitWorker_w);
    _constructionUnit_w->PlaceUnitOnMap();
}

void GUConstructBuildingTask::FinishTask()
{
    _constructionUnit_w = NULL;
}

bool GUConstructBuildingTask::NeedUserInteractionToFinish()
{
    return true;
}

GameUnit* GUConstructBuildingTask::GetSecondObject()
{
    return _constructionUnit_w;
}

