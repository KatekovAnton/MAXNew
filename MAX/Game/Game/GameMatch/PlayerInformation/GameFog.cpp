//
//  GameFog.cpp
//  MAX
//
//  Created by Eugene Fominykh on 3/24/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameFog.h"
#include "GameUnit.h"

GameFog::GameFog(int mapWidth, int mapHeight)
{
    _mapWidth = mapWidth;
    _mapHeight = mapHeight;
    _mapSize = _mapWidth * _mapHeight;
    
    // create an empty field
    _gameField = new int[_mapSize];
    memset(_gameField, 0, _mapSize);
}

GameFog::~GameFog()
{
    delete [] _gameField;
}

void GameFog::Recount(GameUnit *unit, CCPoint unitPosition, bool withIncreasing)
{
    BoundingBox box = unit->GetScanBoundingBox(unitPosition);
    CCPoint scannedPoint = ccp(box.min.x, box.min.y);
    
    for (int x = box.min.x; x <= box.max.x; x++)
    {
        for (int y = box.min.y; y <= box.max.y; y++)
        {
            scannedPoint.setPoint(x, y);
            
            if (unit->IsInScanRadius(scannedPoint))
            {
                if (withIncreasing)
                    Increase(scannedPoint);
                else
                    Decrease(scannedPoint);
            }
        }
    }
    
}

void GameFog::Update(GameUnit *unit, CCPoint unitPosition)
{
    Recount(unit, unitPosition, true);
}

void GameFog::Reset(GameUnit *unit, CCPoint unitPosition)
{
    Recount(unit, unitPosition, false);
}

void GameFog::Increase(const CCPoint& point)
{
    _gameField[IndexOf(point)]++;
}

void GameFog::Decrease(const CCPoint& point)
{
    if (GetValue(point) > 0) {
        _gameField[IndexOf(point)]--;
    }
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