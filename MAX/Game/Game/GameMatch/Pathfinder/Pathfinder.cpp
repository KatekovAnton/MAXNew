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
#include "GameMap.h"
#include "MAXObjectConfig.h"

Pathfinder::Pathfinder(GameMap* _map)
{
    moveType = UNIT_MOVETYPE_GROUND;
    map = _map;
    w = map->_w;
    h = map->_h;
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

void Pathfinder::MakePathMap(const int fromX, const int fromY, UNIT_MOVETYPE unitMoveType) // Fill path map. Used if need to get cost for whole map
{
    moveType = unitMoveType;
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

std::vector<PFWaveCell*> Pathfinder::FindPath(const int fromX, const int fromY, const int toX, const int toY, UNIT_MOVETYPE unitMoveType)
{
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
    return;
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

int Pathfinder::GetCost(const int x, const int y, unsigned char direction)
{
    int result = -1;
    if ((x >= 0) && (y >= 0) && (x < w) && (y < h))
    {
        GROUND_TYPE groundType = map->GroundTypeAtXY(x, y);
        switch (moveType)
        {
            case UNIT_MOVETYPE_GROUND:
            {
                if (groundType == GROUND_TYPE_GROUND)
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_GROUNDCOAST:
            {
                if ((groundType == GROUND_TYPE_GROUND) || (groundType == GROUND_TYPE_COAST))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_AMHIB:
            {
                if ((groundType == GROUND_TYPE_GROUND) || (groundType == GROUND_TYPE_COAST))
                {
                    result = 10;
                }
                else if (groundType == GROUND_TYPE_WATER)
                {
                    result = 30;
                }
                break;
            }
            case UNIT_MOVETYPE_SEACOAST:
            {
                if ((groundType == GROUND_TYPE_WATER) || (groundType == GROUND_TYPE_COAST))
                {
                    result = 10;
                }
                break;
            }
            case UNIT_MOVETYPE_SEA:
            {
                if (groundType == GROUND_TYPE_WATER)
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
            if (moveType == UNIT_MOVETYPE_AIR)
            {
                if (false) // TBD: check if some AIR unit
                {
                    result = -1;
                }
            }
            else if (false) // TBD: check if some NON AIR unit or building exist
            {
                result = -1;
            }
            else if (
                     (
                      (moveType == UNIT_MOVETYPE_GROUND) ||
                      (moveType == UNIT_MOVETYPE_GROUNDCOAST) ||
                      (moveType == UNIT_MOVETYPE_AMHIB)
                     ) &&
                     false // TBD: check if road or bridge exist
                    )
            {
                result /= 2;
            }

            if (direction % 2 == 1)
            {
                result *= 1.4142;
            }
        }
    }
    
    return result;
}

void Pathfinder::TestNeighbours(void)
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
		cost = GetCost(x, y, d);
		if ((cost >= 0) && (pathMap[x + y * w].distance < 0))
		{
			newWave->Add(pos, cost, d);
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

int Pathfinder::GetIndexAt(const int x, const int y)
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
    TestNeighbours();
    
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
                                cell->cost = cell2->cost + GetCost(pos.x, pos.y, item->direction);
                                cell->direction = item->direction;
                                finished = ((item->x == toX) && (item->y == toY));
                                if (finished)
                                {
                                    break;
                                }
                                TestNeighbours();
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
        cost = GetCost(x, y, direction);
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
