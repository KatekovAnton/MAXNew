//
//  MatchMapAgregator.cpp
//  MAX
//
//  Created by Anton Katekov on 01.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MatchMapAgregator.h"
#include "GameMap.h"
#include "GameFog.h"
#include "GameUnit.h"
#include "GameUnitParameters.h"
#include "GameUnitCurrentState.h"
#include "GameMatchPlayer.h"

inline EXTENDED_GROUND_TYPE TranslateGroundType(GROUND_TYPE t)
{
    switch (t) {
        case GROUND_TYPE_COAST:
            return EXTENDED_GROUND_TYPE_COAST;
            break;
        case GROUND_TYPE_GROUND:
            return EXTENDED_GROUND_TYPE_GROUND;
            break;
        case GROUND_TYPE_UNPASSABLE:
            return EXTENDED_GROUND_TYPE_UNPASSABLE;
            break;
        case GROUND_TYPE_WATER:
            return EXTENDED_GROUND_TYPE_WATER;
            break;
            
        default:
            break;
    }
}

int MatchMapAgregator::GetIndexForCoordinates(const int x, const int y) const
{
    return y * _map_w->_w + x;
}

USimpleContainer<GameUnit*> *MatchMapAgregator::UnitsInCell(const int x, const int y) const
{
    return _unitsInCells_w[GetIndexForCoordinates(x, y)];
}

MatchMapAgregator::MatchMapAgregator(GameMap* map)
:_map_w(map), _unitsInCells_w(new USimpleContainer<GameUnit*>*[map->_w * map->_h]), _mapBuffer(new EXTENDED_GROUND_TYPE[map->_w * map->_h]), _mapOutputBuffer(new EXTENDED_GROUND_TYPE[map->_w * map->_h])
{
    int size = map->_w * map->_h;
    for (int i = 0; i < size; i++)
        _unitsInCells_w[i] = new USimpleContainer<GameUnit*>(4);
    for (int i = 0; i < size; i++)
        _mapBuffer[i] = TranslateGroundType(_map_w->GroundTypeAtIndex(i));
    memcpy(_mapBuffer, _mapOutputBuffer, size * sizeof(EXTENDED_GROUND_TYPE));
}

MatchMapAgregator::~MatchMapAgregator()
{
    for (int i = 0; i < _map_w->_w * _map_w->_h; i++)
        delete _unitsInCells_w[i];
    delete [] _unitsInCells_w;
    delete _mapOutputBuffer;
    delete _mapBuffer;
}

void MatchMapAgregator::RemoveUnitFromCell(GameUnit *unit, const int x, const int y)
{
    UnitsInCell(x,y)->removeObject(unit);
    if (unit->_unitCurrentParameters->_unitBaseParameters->GetIsBuilding() && unit->_unitCurrentParameters->_unitBaseParameters->GetSize() == 2)
    {
        UnitsInCell(x, y+1)->removeObject(unit);
        UnitsInCell(x+1, y)->removeObject(unit);
        UnitsInCell(x+1, y+1)->removeObject(unit);
    }
}

void MatchMapAgregator::AddUnitToCell(GameUnit *unit, const int x, const int y)
{
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    if (units->indexOf(unit) == -1)
        units->addObject(unit);
    if (unit->_unitCurrentParameters->_unitBaseParameters->GetIsBuilding() && unit->_unitCurrentParameters->_unitBaseParameters->GetSize() == 2)
    {
        units = UnitsInCell(x, y+1);
        if (units->indexOf(unit) == -1)
            units->addObject(unit);
        
        units = UnitsInCell(x+1, y);
        if (units->indexOf(unit) == -1)
            units->addObject(unit);
        
        units = UnitsInCell(x+1, y+1);
        if (units->indexOf(unit) == -1)
            units->addObject(unit);
    }
}

GameUnit* MatchMapAgregator::GetUnitInPosition(const int x, const int y)
{
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    if (units->GetCount() == 0) 
        return NULL;
    return units->objectAtIndex(0);
}

EXTENDED_GROUND_TYPE *MatchMapAgregator::CalculateFieldForPlayer(GameMatchPlayer *player, GameUnit *unit)
{
    return _mapOutputBuffer;
}

