//
//  MatchGarbageMap.cpp
//  MAX
//
//  Created by  Developer on 03.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MatchGarbageMap.h"
#include "GameMap.h"
#include "GameEffect.h"
#include "MatchMapAgregator.h"

int MatchGarbageMap::GetIndexForCoordinates(const int x, const int y) const
{
    return y * _map_w->_w + x;
}

MatchGarbageMap::MatchGarbageMap(GameMap *map, MatchMapAgregator *agregator)
:_map_w(map), _map_agregator(agregator), _garbageInCells(new USimpleContainer<GarbageElement*>*[map->_w * map->_h]), _effects(new USimpleContainer<GameEffect*>(3))
{
    size = map->_w * map->_h;
    for (int i = 0; i < size; i++)
        _garbageInCells[i] = new USimpleContainer<GarbageElement*>(1);
}

MatchGarbageMap::~MatchGarbageMap()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < _garbageInCells[i]->GetCount(); j++) 
            delete _garbageInCells[i]->objectAtIndex(j);
        
        delete _garbageInCells[i];
    }
    delete []_garbageInCells;
    
    for (int i = 0; i < _effects->GetCount(); i++)
    {
        _effects->objectAtIndex(i)->Hide();
        delete _effects->objectAtIndex(i);
    }
    delete _effects;
}

vector<GameEffect*> MatchGarbageMap::EffectsInCell(const int x, const int y)
{
    USimpleContainer<GarbageElement*> *effects = _garbageInCells[GetIndexForCoordinates(x, y)];
    vector<GameEffect *> result;
    for (int i = 0; effects->GetCount(); i++)
        result.push_back(effects->objectAtIndex(i)->_effect_w);
    
    return result;
}

const GarbageElement *MatchGarbageMap::GarbageInCell(const int x, const int y)
{
    USimpleContainer<GarbageElement*> *effects = _garbageInCells[GetIndexForCoordinates(x, y)];
    if (effects->GetCount() > 0) 
        return effects->objectAtIndex(effects->GetCount() - 1);
    
    return NULL;
}

bool MatchGarbageMap::ContainGarbageInCell(const int x, const int y)
{
    USimpleContainer<GarbageElement*> *effects = _garbageInCells[GetIndexForCoordinates(x, y)];
    return effects->GetCount() > 0;
}

void MatchGarbageMap::AddGarbageToCell(const int x, const int y, const int size, const int rawMat)
{
    bool canCreateGarbage = true;
    vector<CCPoint> points;
    points.push_back(ccp(x, y));
    if (size > 1)
    {
         points.push_back(ccp(x+1, y));
         points.push_back(ccp(x, y+1));
         points.push_back(ccp(x+1, y+1));
    }
    for (int i = 0; i < points.size(); i++) {
        CCPoint point = points[i];
        
        EXTENDED_GROUND_TYPE t = _map_agregator->GroundTypeAtXY(point.x, point.y);
        if (t == EXTENDED_GROUND_TYPE_GROUND || t == EXTENDED_GROUND_TYPE_ROAD)
            continue;
        if (t == EXTENDED_GROUND_TYPE_WATER || t == EXTENDED_GROUND_TYPE_COAST || t == EXTENDED_GROUND_TYPE_UNPASSABLE) {
            canCreateGarbage = false;
            break;
        }
        if (t == EXTENDED_GROUND_TYPE_BRIDGE) {
            
        }
    }
    
    if (canCreateGarbage) 
    {
        GarbageElement *newElement = new GarbageElement();
        newElement->_x = x;
        newElement->_y = y;
        newElement->_size = size;
        newElement->_rawMat = rawMat;
    }
}

bool MatchGarbageMap::ModifyGarbageInCell(const GarbageElement* element, const int deltaRawMat)
{
    return false;
}

