//
//  GameFog.cpp
//  MAX
//
//  Created by Eugene Fominykh on 3/24/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameFog.h"
#include "GameUnit.h"
#include "GameFogDelegate.h"

GameFog::GameFog(int mapWidth, int mapHeight)
:_delegate_w(NULL)
{
    _mapWidth = mapWidth;
    _mapHeight = mapHeight;
    _mapSize = _mapWidth * _mapHeight;
    
    // create an empty field
    _gameField = new int[_mapSize];
    _gameFieldLast = new int[_mapSize];
    memset(_gameField, 0, _mapSize * sizeof(int));
    memset(_gameFieldLast, 0, _mapSize  * sizeof(int));
}

GameFog::~GameFog()
{
    delete [] _gameField;
    delete [] _gameFieldLast;
}

void GameFog::Recount(GameUnit *unit, bool withIncreasing, const CCPoint &centerPoint)
{
    BoundingBox box = unit->GetScanBoundingBox(centerPoint);
    if (box.min.x < 0)
        box.min.x = 0;
    if (box.min.y < 0)
        box.min.y = 0;
    if (box.max.x >= _mapWidth-1)
        box.max.x = _mapWidth-1;
    if (box.max.y >= _mapHeight-1)
        box.max.y = _mapHeight-1;
    
    
    CCPoint scannedPoint = ccp(box.min.x, box.min.y);
    float radius = 0;
    if(_delegate_w)
        radius = _delegate_w->UnitScanRadiusForFog(unit, this);
    for (int x = box.min.x; x <= box.max.x; x++)
    {
        for (int y = box.min.y; y <= box.max.y; y++)
        {
            scannedPoint.setPoint(x, y);
            if (unit->IsInRadius(scannedPoint, radius, centerPoint))
            {
                if (withIncreasing)
                    Increase(scannedPoint);
                else
                    Decrease(scannedPoint);
            }
        }
    }
}

void GameFog::Update(GameUnit *unit, const CCPoint &centerPoint)
{
    Recount(unit, true, centerPoint);
}

void GameFog::Reset(GameUnit *unit, const CCPoint &centerPoint)
{
    Recount(unit, false, centerPoint);
}

void GameFog::Increase(const CCPoint& point)
{
    _gameField[IndexOf(point)]++;
}

void GameFog::Decrease(const CCPoint& point)
{
    _gameField[IndexOf(point)]--;
}

bool GameFog::IsInTouchZone(GameUnit *unit) const
{
    CCPoint point = unit->GetUnitCell();
    return (bool) _gameField[IndexOf(point)];
}

int GameFog::GetValue(const CCPoint &point) const
{
    return _gameField[IndexOf(point)];
}

void GameFog::BeginUpdates()
{
    memcpy(_gameFieldLast, _gameField, sizeof(int) * _mapSize);
}

void GameFog::EndUpdates()
{
    if (_updatingBox.min.x < 0) 
        _updatingBox.min.x = 0;
    if (_updatingBox.min.y < 0)
        _updatingBox.min.y = 0;
    if (_updatingBox.max.x >= _mapWidth-1)
        _updatingBox.max.x = _mapWidth-1;
    if (_updatingBox.max.y >= _mapHeight-1)
        _updatingBox.max.y = _mapHeight-1;
    
    for (int x = _updatingBox.min.x; x <= _updatingBox.max.x; x++)
    {
        for (int y = _updatingBox.min.y; y <= _updatingBox.max.y; y++)
        {
            int idx = IndexOf(x, y);
            if ((_gameField[idx] == 0) != (_gameFieldLast[idx] == 0) && _delegate_w)
            {
                int value = _gameField[idx];
                bool flag = value != 0;
                if (_delegate_w)
                    _delegate_w->CellDidUpdate(x, y, this, flag);
            }
        }
    }
}

void GameFog::UpdateOnUnitDidStartMove(GameUnit* unit)
{
    BeginUpdates();
    _updatingBox = unit->GetScanBoundingBox(unit->GetUnitCell());
    Reset(unit, unit->GetUnitCell());
}

void GameFog::UpdateOnUnitDidEndMove(GameUnit* unit)
{
    Update(unit, unit->GetUnitCell());
    _updatingBox.AddBoundingBox(unit->GetScanBoundingBox(unit->GetUnitCell()));
    EndUpdates();
}

void GameFog::UpdateOnUnitDidPlaceToMap(GameUnit* unit)
{
    BeginUpdates();
    _updatingBox = unit->GetScanBoundingBox();
    Update(unit, unit->GetUnitCell());
    EndUpdates();
}

void GameFog::UpdateOnUnitDidRemoveFromMap(GameUnit* unit)
{
    BeginUpdates();
    _updatingBox = unit->GetScanBoundingBox();
    Reset(unit, unit->GetUnitCell());
    EndUpdates();
}




