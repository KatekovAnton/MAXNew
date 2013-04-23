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
#include "MAXObjectConfig.h"

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

EXTENDED_GROUND_TYPE MatchMapAgregator::GroundTypeAtXY(const int x, const int y) const
{
    return _mapOutputBuffer[GetIndexForCoordinates(x, y)];
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
    memcpy(_mapOutputBuffer, _mapBuffer, size * sizeof(EXTENDED_GROUND_TYPE));
}

MatchMapAgregator::~MatchMapAgregator()
{
    for (int i = 0; i < _map_w->_w * _map_w->_h; i++)
        delete _unitsInCells_w[i];
    delete [] _unitsInCells_w;
    delete _mapOutputBuffer;
    delete _mapBuffer;
}

int MatchMapAgregator::Width()
{
    return _map_w->_w;
}

int MatchMapAgregator::Height()
{
    return _map_w->_h;
}

#pragma mark - unit events

void MatchMapAgregator::RemoveUnitFromCell(GameUnit *unit, const int x, const int y)
{
    UnitsInCell(x,y)->removeObject(unit);
	MAXObjectConfig* config = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig();
	if (config->_isPlatform || config->_isBridge)
	{
		int idx = GetIndexForCoordinates(x, y);
		_mapOutputBuffer[idx] = _mapBuffer[idx];
	}
	else if (config->_isRoad)
	{
		int idx = GetIndexForCoordinates(x, y);
		 
		// Check if no more platform exist in the cell. It is possible construct road above platform
		bool platformExist = false;
		USimpleContainer<GameUnit*> *units = _unitsInCells_w[idx];
		for (int i = 0; i < units->GetCount(); i++)
		{
			GameUnit *unit2 = units->objectAtIndex(i);
			config = unit2->_unitCurrentParameters->_unitBaseParameters->GetConfig();
			if (config->_isPlatform)
			{
				platformExist = true;
				break;
			}
		}
		if (!platformExist)
		{
			_mapOutputBuffer[idx] = _mapBuffer[idx];
		}
	}

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
	MAXObjectConfig* config = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig();
	if (config->_isRoad || config->_isPlatform)
	{
		int idx = GetIndexForCoordinates(x, y);
		_mapOutputBuffer[idx] = EXTENDED_GROUND_TYPE_ROAD;
	}
	else if (config->_isBridge)
	{
		int idx = GetIndexForCoordinates(x, y);
		_mapOutputBuffer[idx] = EXTENDED_GROUND_TYPE_BRIDGE;		
	}
	
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

GameUnit* MatchMapAgregator::GetUnitInPosition(const int x, const int y, GameMatchPlayer *_player, bool selectedUnit)
{
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    if (units->GetCount() == 0)
        return NULL;
	GameUnit* result = NULL;
    for (int i = 0; i < units->GetCount(); i++) 
	{
        if ((!_player) || (units->objectAtIndex(i)->_owner_w == _player))
		{
			GameUnit* unit = units->objectAtIndex(i);
			MAXObjectConfig* config = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig();
			if (selectedUnit)
			{
				if (!config->_isCantSelect)
				{
					result = unit;
					break;
				}
			}
			else
			{
				result = unit;
				if (!config->_isCantSelect)
				{
					break;
				}
			}		
		}
    }
    return result;
}

bool MatchMapAgregator::IsGroundUnitInPosition(const int x, const int y)
{
    bool result = false;
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    for (int i = 0; i < units->GetCount(); i++)
    {
        GameUnit* unit = units->objectAtIndex(i);
		MAXObjectConfig* config = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig();
        if ((config->_bMoveType != UNIT_MOVETYPE_AIR) && (!config->_isRoad) && (!config->_isBridge) && (!config->_isPlatform) && (!config->_isConnector))
        {
            result = true;
            break;
        }
    }
    return result;
}

bool MatchMapAgregator::IsAirUnitInPosition(const int x, const int y)
{
    bool result = false;
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    for (int i = 0; i < units->GetCount(); i++)
    {
        GameUnit* unit = units->objectAtIndex(i);
        if (unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType == UNIT_MOVETYPE_AIR)
        {
            result = true;
            break;
        }
    }
    return result;
}

EXTENDED_GROUND_TYPE *MatchMapAgregator::CalculateFieldForPlayer(GameMatchPlayer *player, GameUnit *unit)
{
    return _mapOutputBuffer;
}

