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
#include "GameUnitData.h"
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
    return _mapBuffer[GetIndexForCoordinates(x, y)];
}

USimpleContainer<GameUnit*> *MatchMapAgregator::UnitsInCell(const int x, const int y) const
{
    USimpleContainer<GameUnit*> *results = _unitsInCells_w[GetIndexForCoordinates(x, y)];
    return results;
}

MatchMapAgregator::MatchMapAgregator(GameMap* map)
:_map_w(map), _unitsInCells_w(new USimpleContainer<GameUnit*>*[map->_w * map->_h]), _mapBuffer(new EXTENDED_GROUND_TYPE[map->_w * map->_h])
{
    int size = map->_w * map->_h;
    for (int i = 0; i < size; i++)
        _unitsInCells_w[i] = new USimpleContainer<GameUnit*>(4);
    for (int i = 0; i < size; i++)
        _mapBuffer[i] = TranslateGroundType(_map_w->GroundTypeAtIndex(i));
}

MatchMapAgregator::~MatchMapAgregator()
{
    for (int i = 0; i < _map_w->_w * _map_w->_h; i++)
        delete _unitsInCells_w[i];
    delete [] _unitsInCells_w;
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

void MatchMapAgregator::ClearAllData()
{
    int size = _map_w->_w * _map_w->_h;
    for (int i = 0; i < size; i++)
    {
        USimpleContainer<GameUnit*> *units = _unitsInCells_w[i];
        for (int j = 0; j < units->GetCount(); j++)
        {
            GameUnit* unit = units->objectAtIndex(j);
            unit->Hide();
        }
        units->clear();
    }
    for (int i = 0; i < size; i++)
        _mapBuffer[i] = TranslateGroundType(_map_w->GroundTypeAtIndex(i));
}

void MatchMapAgregator::UpdateBridgeAt(const int x, const int y, bool isLift)
{
	USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
	for (int i = 0; i < units->GetCount(); i++)
	{
		GameUnit *unit2 = units->objectAtIndex(i);
		if (unit2->_unitData->GetConfig()->_isBridge)
		{
			if (isLift)
				unit2->LiftBridge();
			else
				unit2->DropBridge();
			break;
		}
	}
}

#pragma mark - unit events

void MatchMapAgregator::RemoveUnitFromCell(GameUnit *unit, const int x, const int y)
{
    UnitsInCell(x,y)->removeObject(unit);
	MAXObjectConfig* config = unit->_unitData->GetConfig();
	int idx = GetIndexForCoordinates(x, y);
	if (config->_isPlatform || config->_isBridge)
	{
		_mapBuffer[idx] = TranslateGroundType(_map_w->GroundTypeAtXY(x, y));// _mapBuffer[idx];
	}
	else if (config->_isRoad)
	{
		// Check if no more platform exist in the cell. It is possible construct road above platform
		bool platformExist = false;
		USimpleContainer<GameUnit*> *units = _unitsInCells_w[idx];
		for (int i = 0; i < units->GetCount(); i++)
		{
			GameUnit *unit2 = units->objectAtIndex(i);
			config = unit2->_unitData->GetConfig();
			if (config->_isPlatform)
			{
				platformExist = true;
				break;
			}
		}
		if (!platformExist)
		{
			_mapBuffer[idx] = TranslateGroundType(_map_w->GroundTypeAtXY(x, y));//_mapBuffer[idx];
		}
	}

	if ((_mapBuffer[idx] == EXTENDED_GROUND_TYPE_BRIDGE) && (config->_bMoveType == UNIT_MOVETYPE_SEACOAST || config->_bMoveType == UNIT_MOVETYPE_SEA) && !config->_isBridge)
	{
		UpdateBridgeAt(x, y, false);
	}

    if (unit->_unitData->GetIsBuilding() && unit->_unitData->GetSize() == 2)
    {
        UnitsInCell(x, y+1)->removeObject(unit);
        UnitsInCell(x+1, y)->removeObject(unit);
        UnitsInCell(x+1, y+1)->removeObject(unit);
    }
}

void MatchMapAgregator::AddUnitToCell(GameUnit *unit, const int x, const int y)
{
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
	MAXObjectConfig* config = unit->_unitData->GetConfig();
	int idx = GetIndexForCoordinates(x, y);
	if ((_mapBuffer[idx] == EXTENDED_GROUND_TYPE_BRIDGE) && (config->_bMoveType == UNIT_MOVETYPE_SEACOAST || config->_bMoveType == UNIT_MOVETYPE_SEA) && !config->_isBridge)
	{
		UpdateBridgeAt(x, y, true);
	}
	if (units->indexOf(unit) == -1)
		units->addObject(unit);

	if (config->_isRoad || config->_isPlatform)
	{
		_mapBuffer[idx] = EXTENDED_GROUND_TYPE_ROAD;
	}
	else if (config->_isBridge)
	{
		_mapBuffer[idx] = EXTENDED_GROUND_TYPE_BRIDGE;		
	}
	
    if (unit->_unitData->GetIsBuilding() && unit->_unitData->GetSize() == 2)
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

GameUnit* MatchMapAgregator::GetUnitInPosition(const int x, const int y, GameMatchPlayer *_player, GameUnit* selectedUnit, bool forceAnyone)
{
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    if (units->GetCount() == 0)
        return NULL;

    bool findNextUnitInCell = false;
    if (selectedUnit != NULL)
        findNextUnitInCell = selectedUnit->_unitData->IsCellOfUnit(ccp(x, y));
    
    bool selectedUnitFound = false;
    
	GameUnit* result = NULL;
    for (int i = 0; i < units->GetCount(); i++) 
	{
        if ((!_player) || (units->objectAtIndex(i)->_owner_w == _player))
		{
			GameUnit* unit = units->objectAtIndex(i);
            if (unit->_unitData->_isConstruction)
                continue;
            
			MAXObjectConfig* config = unit->_unitData->GetConfig();
			if (selectedUnit != NULL)
			{
                if (findNextUnitInCell)
                {
                    if (selectedUnitFound) // next unit
                    {
                        result = unit;
                        break;
                    }
                    else if (selectedUnit == unit)
                    {
                        selectedUnitFound = true;
                        if (result == NULL)
                        {
                            result = selectedUnit; // if no more units
                        }
                    }
                    else if (result == NULL) // first unit
                    {
                        result = unit;
                    }
                }
                else
                {
                    MAXObjectConfig* selectedConfig = selectedUnit->_unitData->GetConfig();
                    if ((!config->_isCantSelect) || selectedConfig->_isCantSelect || selectedConfig->_isBuilding || (forceAnyone && !result))
                    {
                        result = unit;
                        break;
                    }
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
		MAXObjectConfig* config = unit->_unitData->GetConfig();
        if ((config->_bMoveType != UNIT_MOVETYPE_AIR) &&
            (!config->_isRoad) &&
            (!config->_isBridge) &&
            (!config->_isPlatform) &&
            (!config->_isConnector) &&
            (!config->_isBombMine) &&
            (!unit->_currentlyProcesedConstructor)
           )
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
        if (unit->_unitData->GetConfig()->_bMoveType == UNIT_MOVETYPE_AIR)
        {
            result = true;
            break;
        }
    }
    return result;
}

bool MatchMapAgregator::ContainConnectoredBuildingInPosition(const int x, const int y, GameMatchPlayer *_player) const
{
    bool result = false;
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    for (int i = 0; i < units->GetCount(); i++)
    {
        GameUnit* unit = units->objectAtIndex(i);
        if (unit->_unitData->GetIsConnectored() && !unit->_unitData->_isConstruction && unit->_owner_w == _player)
        {
            result = true;
            break;
        }
    }
    return result;
}

void MatchMapAgregator::FindAllConnectoredUnits(const int x, const int y, GameMatchPlayer *_player, USimpleContainer<GameUnit*> *buffer)
{
    USimpleContainer<GameUnit*> *units = UnitsInCell(x, y);
    for (int i = 0; i < units->GetCount(); i++)
    {
        GameUnit* unit = units->objectAtIndex(i);
        
        if (unit->_unitData->GetIsConnectored() && unit->_owner_w == _player && buffer->indexOf(unit) == -1)
            buffer->addObject(unit);
    }
}
