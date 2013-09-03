//
//  Pathfinder.cpp
//  MAX
//
//  Created by Alexey Malyshko on 3/27/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "Pathfinder.h"
#include "PFWave.h"
#include "PFWaveCell.h"
#include "MatchMapAgregator.h"
#include "MAXObjectConfig.h"

Pathfinder::Pathfinder(MatchMapAgregator* _map)
{
    moveType = UNIT_MOVETYPE_GROUND;
    map = _map;
    w = map->Width();
    h = map->Height();
    pathMap = (PFPathMapCell*)malloc(w * h * sizeof(PFPathMapCell));
    oldWave = new PFWave();
    newWave = new PFWave();
}

Pathfinder::~Pathfinder()
{
    if (pathMap)
        free(pathMap);
    if (oldWave)
        delete oldWave;
    if (newWave)
        delete newWave;
}

void Pathfinder::MakePathMap(const int fromX, const int fromY, UNIT_MOVETYPE unitMoveType, int maxCost) // Fill path map. Used if need to get cost for whole map
{
    moveType = unitMoveType;
    _maxCost = maxCost;
    FillPathMap(fromX, fromY, -1, -1);
}

int Pathfinder::GetCostAt(const int x, const int y) // makePathMap should be called first
{
    int result = -1;
    int ind = GetIndexAt(x, y);
    if (ind >= 0)
    {
        PFPathMapCell* cell = &(pathMap[ind]);
        result = cell->cost;
    }
    return result;
}

int Pathfinder::GetMapCostAt(const int x, const int y, unsigned char direction, UNIT_MOVETYPE unitMoveType)
{
    int result = GetCost(x, y, direction, unitMoveType);
    return result;
}

std::vector<PFWaveCell*> Pathfinder::FindPath(const int fromX, const int fromY, const int toX, const int toY, UNIT_MOVETYPE unitMoveType)
{
    _maxCost = -1;    
    moveType = unitMoveType;
    
    std::vector<PFWaveCell*> result;

    int indFrom = GetIndexAt(fromX, fromY);
    int indTo   = GetIndexAt(toX, toY);
    
    if ((indFrom >= 0) && (indTo >= 0))
    {
        FillPathMap(fromX, fromY, toX, toY);
        result = FindPathOnMap(toX, toY);
    }
    
    return result;
}

void Pathfinder::DumpMap(void)
{
    printf("PathMap:\n");
#if 0
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            PFPathMapCell* cell = &(pathMap[x + y * w]);
            char c[50];
            if (cell->distance == 0)
            {
                sprintf(c, "  S");
            }
            else if (cell->distance < 0)
                sprintf(c, "  #");
            else
                sprintf(c, "%3d", cell->cost / 10);
            printf("%s", c);
        }
        printf("\n");
    }
#else
    int x0 = _fromX - 25; if (x0 < 0) x0 = 0; if (x0 > w) x0 = w;
    int y0 = _fromY - 25; if (y0 < 0) y0 = 0; if (y0 > h) y0 = h;
    int x1 = _fromX + 25; if (x1 < 0) x1 = 0; if (x1 > w) x1 = w;
    int y1 = _fromY + 25; if (y1 < 0) y1 = 0; if (y1 > h) y1 = h;
    for (int y = y0; y < y1; y++)
    {
        for (int x = x0; x < x1; x++)
        {
            PFPathMapCell* cell = &(pathMap[x + y * w]);
            char c[10];
            if (cell->distance == 0)
            {
                sprintf(c, " S");
            }
            else if (cell->distance < 0)
                sprintf(c, " #");
            else
                sprintf(c, " +");
            printf("%s", c);
        }
        printf("\n");
    }
#endif
}

void Pathfinder::PreparePathMap(void)
{
    for (int i = 0; i < w * h; i++)
    {
        PFPathMapCell* cell = &(pathMap[i]);
        cell->distance = -1;
        cell->cost = 0;
        cell->direction = 0;
    }
}

int Pathfinder::GetCost(const int x, const int y, unsigned char direction, UNIT_MOVETYPE unitMoveType)
{
    int result = -1;
    if ((x >= 0) && (y >= 0) && (x < w) && (y < h))
    {
        EXTENDED_GROUND_TYPE groundType = map->GroundTypeAtXY(x, y);
        switch (unitMoveType)
        {
            case UNIT_MOVETYPE_GROUND:
            {
                if (groundType == EXTENDED_GROUND_TYPE_ROAD)
                {
                    result = 5;
                }
                else if ((groundType == EXTENDED_GROUND_TYPE_GROUND)||
                         (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_GROUNDCOAST:
            {
                if (groundType == EXTENDED_GROUND_TYPE_ROAD) 
                {
                    result = 5;
                }
                else if ((groundType == EXTENDED_GROUND_TYPE_GROUND) ||
                         (groundType == EXTENDED_GROUND_TYPE_COAST)  ||
                         (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_SURVEYOR:
            {
                if (groundType == EXTENDED_GROUND_TYPE_ROAD)
                {
                    result = 5;
                }
                else if ((groundType == EXTENDED_GROUND_TYPE_GROUND) ||
                         (groundType == EXTENDED_GROUND_TYPE_COAST)  ||
                         (groundType == EXTENDED_GROUND_TYPE_WATER)  ||
                         (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_AMHIB:
            {
                if (groundType == EXTENDED_GROUND_TYPE_ROAD) 
                {
                    result = 5;
                }
                else if ((groundType == EXTENDED_GROUND_TYPE_GROUND) ||
                         (groundType == EXTENDED_GROUND_TYPE_COAST)  ||
                         (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                {
                    result = 10;
                }
                else if (groundType == EXTENDED_GROUND_TYPE_WATER)
                {
                    result = 30;
                }
                break;
            }
            case UNIT_MOVETYPE_SEACOAST:
            {
                if ((groundType == EXTENDED_GROUND_TYPE_WATER) ||
                    (groundType == EXTENDED_GROUND_TYPE_COAST) ||
                    (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_SEA:
            {
                if ((groundType == EXTENDED_GROUND_TYPE_WATER) ||
                    (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_AIR:
            {
                result = 10;
                break;
            }
        }
        
        if (result >= 0)
        {
            if (unitMoveType == UNIT_MOVETYPE_AIR)
            {
                if (map->IsAirUnitInPosition(x, y))
                {
                    result = -1;
                }
            }
            else
            {
                if (map->IsGroundUnitInPosition(x, y))
                {
                    result = -1;
                }
            }

            if (direction % 2 == 1)
            {
                result *= 1.4142;
            }
        }
    }
    
    return result;
}

void Pathfinder::TestNeighbours(const int baseCost)
{
	CellPoint pos;
	PFWaveCell* item;
	int cost, x, y;
	for (unsigned char d = 0; d <= 7; d++)
	{
		item = oldWave->GetItem();
		pos = item->GetPosForDirection(d);
        x = pos.x;
        y = pos.y;
		cost = GetCost(x, y, d, moveType);
		if ((cost >= 0) && (pathMap[x + y * w].distance < 0))
		{
            if ((_maxCost < 0) || (baseCost + cost <= _maxCost))
            {
                newWave->Add(pos, cost, d);
            }
		}
	}
}

void Pathfinder::ExchangeWaves(void)
{
	PFWave* wave = oldWave;
	oldWave = newWave;
	newWave = wave;
	newWave->Clear();
}

int Pathfinder::GetIndexAt(const int x, const int y) const
{
    int result = -1;
    if ((x >= 0) && (y >= 0) && (x < w) && (y < h))
    {
        result = x + y * w;
    }
    return result;
}


void Pathfinder::FillPathMap(const int fromX, const int fromY, const int toX, const int toY)
{
    PreparePathMap();
    
    _fromX = fromX;
    _fromY = fromY;
    
    oldWave->Clear();
    newWave->Clear();
    
    int ind = GetIndexAt(fromX, fromY);
    PFPathMapCell* cell = NULL;
    if (ind >= 0)
    {
        cell = &(pathMap[ind]);
        cell->distance = 0;
        cell->cost = 0;
    }
    oldWave->Add(fromX, fromY, 0, 0);
    TestNeighbours(0);
    
    bool finished = ((fromX == toX) && (fromY == toY));
    PFWaveCell* item;

	while (!finished)
	{
        ExchangeWaves();
        if (oldWave->Start())
        {
            do
            {
                item = oldWave->GetItem();
                item->cost -= oldWave->minCost;
                if (item->cost > 0)
                {
                    newWave->Add(item);
                }
                else
                {
                    ind = GetIndexAt(item->x, item->y);
                    if (ind >= 0)
                    {
                        cell = &(pathMap[ind]);
                        if (cell->distance < 0)
                        {
                            CellPoint pos = item->GetPosFrom();
                            int ind2 = GetIndexAt(pos.x, pos.y);
                            if (ind2 >= 0)
                            {
                                PFPathMapCell* cell2 = &(pathMap[ind2]);
                                cell->distance = cell2->distance + 1;
                                pos.x = item->x;
                                pos.y = item->y;
                                int cost = GetCost(pos.x, pos.y, item->direction, moveType);
                                if (cost >= 0)
                                {
                                    cell->cost = cell2->cost + cost;
                                    cell->direction = item->direction;
                                    if (_maxCost < 0)
                                    {
                                        finished = ((item->x == toX) && (item->y == toY));
                                        if (finished)
                                        {
                                            break;
                                        }
                                    }
                                    TestNeighbours(cell->cost);
                                }
                            }
                        }
                    }
                }
            } while (oldWave->Next());
        }
        else
        {
            break;
        }
    }
}

std::vector<PFWaveCell*> Pathfinder::FindPathOnMap(const int toX, const int toY)
{
    std::vector<PFWaveCell*> result;
    int x = toX;
    int y = toY;
    int ind = GetIndexAt(x, y);
    if (ind < 0)
    {
        return result;
    }
    PFPathMapCell* cell = &(pathMap[ind]);
    if ((cell->distance < 0) && (_maxCost >= 0))
    {
        int oldMaxCost = _maxCost;
        result = FindPath(_fromX, _fromY, toX, toY, moveType);
        _maxCost = oldMaxCost;
        return result;
    }
    cell = &(pathMap[ind]);
    if (cell->distance < 0)
    {
        return result;
    }
    
    PFWaveCell* item;
    unsigned char direction;
    int cost;
    while ((ind >= 0) && (cell->distance > 0))
    {
        direction = cell->direction;
        cost = GetCost(x, y, direction, moveType);
        item = new PFWaveCell(x, y, cost, direction);
        result.push_back(item);
        x -= PFWaveCell::DirToDX(direction);
        y -= PFWaveCell::DirToDY(direction);
        ind = GetIndexAt(x, y);
        if (ind >= 0)
        {
            cell = &(pathMap[ind]);
        }
    }

    direction = cell->direction;
    cost = 0;
    item = new PFWaveCell(x, y, cost, direction);
    result.push_back(item);
    
    return result;
}
