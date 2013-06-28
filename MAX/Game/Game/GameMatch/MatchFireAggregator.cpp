//
//  MatchFireAggregator.cpp
//  MAX
//
//  Created by Anton Katekov on 15.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MatchFireAggregator.h"
#include "GameMap.h"
#include "GameUnit.h"
#include "GameUnitData.h"
#include "USimpleContainer.h"
#include "GameMatch.h"
#include "MAXObjectConfig.h"

USimpleContainer<GameUnit*> *MatchFireAggregator::UnitsForCell(const int x, const int y)
{
    USimpleContainer<GameUnit*> *results = _unitsForCells_w[GetIndexForCoordinates(x, y)];
    return results;
}

int MatchFireAggregator::GetIndexForCoordinates(const int x, const int y) const
{
    return y * _map_w->_w + x;
}

MatchFireAggregator::MatchFireAggregator(GameMap *map, GameMatch *match)
:_map_w(map), _match_w(match), _unitsForCells_w(new USimpleContainer<GameUnit*>*[map->_w * map->_h])
{
    int size = map->_w * map->_h;
    for (int i = 0; i < size; i++)
        _unitsForCells_w[i] = new USimpleContainer<GameUnit*>(4);
}

MatchFireAggregator::~MatchFireAggregator()
{
    for (int i = 0; i < _map_w->_w * _map_w->_h; i++)
        delete _unitsForCells_w[i];
    delete [] _unitsForCells_w;
}

int MatchFireAggregator::Width() const
{
    return _map_w->_w;
}

int MatchFireAggregator::Height() const
{
    return _map_w->_h;
}

void MatchFireAggregator::ClearAllData()
{
    int size = _map_w->_w * _map_w->_h;
    for (int i = 0; i < size; i++)
    {
        USimpleContainer<GameUnit*> *units = _unitsForCells_w[i];
        units->clear();
    }
}

void MatchFireAggregator::UnitDidEnterCell(GameUnit *unit, const CCPoint &cell)
{
    if (!unit->GetConfig()->_isAbleToFire)
        return;
    if (unit->_unitData->_isUnderConstruction) 
        return;
    
    
    BoundingBox bb = unit->_unitData->GetBoundingBox(cell, unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_RANGE));
    for (int x = bb.min.x; x <= bb.max.x; x++) {
        for (int y = bb.min.y; y <= bb.max.y; y++) {
            CCPoint cell = ccp(x,y);
            if (unit->_unitData->IsInFireRadius(cell, cell))
            {
                USimpleContainer<GameUnit*> *units = UnitsForCell(x, y);
                if (units->indexOf(unit) == -1) 
                    units->addObject(unit);
            }
        }
    }
}

void MatchFireAggregator::UnitWillLeaveCell(GameUnit *unit, const CCPoint &cell)
{
    if (!unit->GetConfig()->_isAbleToFire) 
        return;
    if (unit->_unitData->_isUnderConstruction)
        return;
    
    BoundingBox bb = unit->_unitData->GetBoundingBox(cell, unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_RANGE));
    for (int x = bb.min.x; x <= bb.max.x; x++) {
        for (int y = bb.min.y; y <= bb.max.y; y++) {
            CCPoint cell = ccp(x,y);
            if (unit->_unitData->IsInFireRadius(cell, cell))
            {
                USimpleContainer<GameUnit*> *units = UnitsForCell(x, y);
                units->removeObject(unit);
            }
        }
    }
}

vector<GameUnit*> MatchFireAggregator::UnitsForAttackingUnitInCell(const int x, const int y, GameUnit* unit)
{
    vector<GameUnit*> result;
    USimpleContainer<GameUnit*> *allUnits = UnitsForCell(x, y);
    for (int i = 0; i < allUnits->GetCount(); i++)
    {
        GameUnit *cUint = allUnits->objectAtIndex(i);
        if (_match_w->UnitCanAttackUnit(cUint, unit) &&
            _match_w->PlayerIsEnemyToPlayer(unit->_owner_w, cUint->_owner_w)) {
            result.push_back(cUint);
        }
    }
    return result;
}

