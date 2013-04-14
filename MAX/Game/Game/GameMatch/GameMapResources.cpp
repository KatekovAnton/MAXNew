//
//  GameMapResources.cpp
//  MAX
//
//  Created by Alexey Malyshko on 3/25/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMapResources.h"
#include "GameMap.h"
#include "GameSettings.h"

GameMapResources::GameMapResources(GameSettings* settings, GameMap* map)
{
    _map = map;
    _settings = settings;
    
    _h = map->_h;
    _w = map->_w;
    
    _resourceValue = (unsigned char*)malloc(_w * _h);
}

GameMapResources::~GameMapResources()
{
    if (_resourceValue)
        free(_resourceValue);
}

RESOURCE_TYPE GameMapResources::GetResourceTypeAt(const int x, const int y) const
{
    RESOURCE_TYPE result;
    if ((y % 2) == 0)
    {
        if ((x % 2) == 0)
        {
            result = RESOURCE_TYPE_FUEL;
        }
        else
        {
            result = RESOURCE_TYPE_NONE;
        }
    }
    else
    {
        if ((x % 2) == 0)
        {
            result = RESOURCE_TYPE_GOLD;
        }
        else
        {
            result = RESOURCE_TYPE_RAW;
        }
    }
    return result;
}

int GameMapResources::GetIndexAt(const int x, const int y) const
{
    int result = -1;
    if ((x >= 0) && (y >= 0) && (x < _w) && (y < _h))
    {
        result = x + y * _w;
    }
    return result;
}

unsigned char GameMapResources::GetResourceValueAt(const int x, const int y) const
{
    int result = 0;
    int resIndex = GetIndexAt(x, y);
    if (resIndex >= 0)
    {
        result = _resourceValue[resIndex];
    }
    return result;
}

typedef enum
{
    P_MIN = 0,
    P_MAX,
    P_COUNT
} P_KIND;

static const int RES_COLLISION_AMOUNT = 7;

static const int RES_DATA[RESOURCE_TYPE_COUNT][RESOURCE_LEVEL_COUNT][RES_MODE_COUNT][P_COUNT] =
{
    //material
    {   // MINE    CONC    NORM    DIFF
        {{10,10},{ 8,12},{ 0, 3},{ 8, 0}}, //RESOURCE_LEVEL_POOR
        {{12,12},{13,16},{ 1, 5},{ 6, 0}}, //RESOURCE_LEVEL_MEDIUM
        {{14,14},{16,16},{ 1, 5},{ 4, 0}}  //RESOURCE_LEVEL_RICH
    },
    //fuel
    {   // MINE    CONC    NORM    DIFF
        {{ 7, 7},{ 8,12},{ 1, 2},{ 8, 0}}, //RESOURCE_LEVEL_POOR
        {{ 8, 8},{12,16},{ 2, 3},{ 6, 0}}, //RESOURCE_LEVEL_MEDIUM
        {{ 9, 9},{16,16},{ 2, 4},{ 4, 0}}  //RESOURCE_LEVEL_RICH
    },
    //gold
    {   // MINE    CONC    NORM    DIFF
        {{ 0, 0},{ 5, 9},{ 0, 0},{12, 0}}, //RESOURCE_LEVEL_POOR
        {{ 0, 0},{ 8,12},{ 0, 0},{10, 0}}, //RESOURCE_LEVEL_MEDIUM
        {{ 1, 1},{12,16},{ 0, 1},{ 8, 0}}  //RESOURCE_LEVEL_RICH
    }
};

unsigned char GameMapResources::GetAmount(RESOURCE_TYPE type, RES_MODE_TYPE mode)
{
    int result = 0;
    if (type != RESOURCE_TYPE_NONE)
    {
        //    rand()%max
        RESOURCE_LEVEL level = _settings->resLevels[type];
        int valMin = RES_DATA[type][level][mode][P_MIN];
        int valMax = RES_DATA[type][level][mode][P_MAX];
        result = valMin;
        int delta = valMax - valMin + 1;
        if (delta > 1)
        {
            delta += rand() % delta;
        }
    }
    return result;
}

unsigned int GameMapResources::GetDiff(RESOURCE_TYPE type)
{
    int result = 0;
    if (type != RESOURCE_TYPE_NONE)
    {
        RESOURCE_LEVEL level = _settings->resLevels[type];
        result = RES_DATA[type][level][RES_MODE_DIFFUSION][P_MIN];
    }
    return result;
}

void GameMapResources::SetAmount(const int x, const int y, const int amount)
{
    int resIndex = GetIndexAt(x, y);
    if (resIndex >= 0)
    {
        _resourceValue[resIndex] = amount;
    }
}

void GameMapResources::MergeAmount(const int x, const int y, const int amount)
{
    int resIndex = GetIndexAt(x, y);
    if (resIndex >= 0)
    {
        int oldAmount = _resourceValue[resIndex];
        if (oldAmount > 0)
        {
            _resourceValue[resIndex] = (amount + oldAmount + 1) / 2;
        }
        else
        {
            _resourceValue[resIndex] = amount;
        }
    }
}

void GameMapResources::AddMinePlacement(const int x, const int y)
{
    bool centerUpdated = false;
    //find material place (center of new resource placement)
    for (int dx = 0; dx <= 1; dx++)
    {
        for (int dy = 0; dy <= 1; dy++)
        {
            RESOURCE_TYPE type = GetResourceTypeAt(x + dx, y + dy);
            if (type == RESOURCE_TYPE_FUEL)
            {
                AddResourcePlacement(x + dx, y + dy);
                centerUpdated = true;
                break;
            }
        }
        if (centerUpdated)
        {
            break;
        }
    }
    
    // update mine station place
    for (int dx = 0; dx <= 1; dx++)
    {
        for (int dy = 0; dy <= 1; dy++)
        {
            RESOURCE_TYPE type = GetResourceTypeAt(x + dx, y + dy);
            if (type != RESOURCE_TYPE_NONE)
            {
                int amount = GetAmount(type, RES_MODE_MINE);
                if (amount > 0)
                {
                    SetAmount(x + dx, y + dy, amount);
                }
            }
        }
    }
}

void GameMapResources::AddResourcePlacement(const int x, const int y)
{
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            RESOURCE_TYPE type = GetResourceTypeAt(x + dx, y + dy);
            if (type != RESOURCE_TYPE_NONE)
            {
                RES_MODE_TYPE mode = RES_MODE_NORMAL;
                if ((dx == 0) && (dy == 0))
                {
                    mode = RES_MODE_CONCENTRATE;
                }
                int amount = GetAmount(type, mode);
                if (amount > 0) // may be remove this checking for merge 0 value
                {
                    MergeAmount(x + dx, y + dy, amount);
                }
            }
        }
    }
}

void GameMapResources::ClearResources(void)
{
    for (int x = 0; x < _w; x++)
    {
        for (int y = 0; y < _h; y++)
        {
            _resourceValue[x + y * _w] = 0;
        }
    }
}

void GameMapResources::GenerateInitialResources(void)
{
    ClearResources();
    int resMax = _settings->resCount * ((_w * _h) / (112 * 112));
    int resCount[RESOURCE_TYPE_COUNT];
    resCount[RESOURCE_TYPE_FUEL] = resMax * 0.40; // 40% of fuel
    resCount[RESOURCE_TYPE_GOLD] = resMax * 0.15; // 15% of gold
    resCount[RESOURCE_TYPE_RAW] = resMax - resCount[RESOURCE_TYPE_FUEL] - resCount[RESOURCE_TYPE_GOLD]; //45% of metal
    
    for (int i = 0; i < resMax; i++)
    {
        for (int type = RESOURCE_TYPE_RAW; type < RESOURCE_TYPE_COUNT; type++)
        {
            if (resCount[type] > 0)
            {
                int okMax = 60;
                bool correctPlace;
                int x;
                int y;
                do
                {
                    correctPlace = true;
                    
                    //fuel cell by default
                    x = (rand() % (_w / 2)) * 2;
                    y = (rand() % (_h / 2)) * 2;
                    switch (type)
                    {
                        case RESOURCE_TYPE_RAW:
                        {
                            x += 1;
                            y += 1;
                            break;
                        }
                        case RESOURCE_TYPE_GOLD:
                        {
                            y += 1;
                            break;
                        }
                        default:
                            break;
                    }
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            int resIndex = GetIndexAt(x + dx, y + dy);
                            if (resIndex >= 0)
                            {
                                GROUND_TYPE gt = _map->GroundTypeAtXY(x + dx, y + dy);
                                if (gt == GROUND_TYPE_UNPASSABLE)
                                {
                                    correctPlace = false;
                                    break;
                                }
                            }
                        }
                        if (!correctPlace)
                        {
                            break;
                        }
                    }
                    if (correctPlace)
                    {
                        //check cells around for same res
						int diff = GetDiff((RESOURCE_TYPE)type);
                        if (okMax < 20)
                        {
                            diff /= 2;
                        }
                        if (okMax < 10)
                        {
                            diff /= 2;
                        }
                        if (diff <= 0)
                        {
                            diff = 1;
                        }
                        if (okMax > 0)
                        {
                            for (int dx = -diff; dx <= diff; dx++)
                            {
                                for (int dy = -diff; dy <= diff; dy++)
                                {
                                    int resIndex = GetIndexAt(x + dx * 2, y + dy * 2);
                                    if (resIndex >= 0)
                                    {
                                        int amount = _resourceValue[resIndex];
                                        if (amount >= RES_COLLISION_AMOUNT)
                                        {
                                            correctPlace = false;
                                            break;
                                        }
                                    }
                                }
                                if (!correctPlace)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    okMax--;
                } while (!correctPlace);

                resCount[type]--;
                AddResourcePlacement(x, y);
            }
        }
    }
}

void GameMapResources::LandPlayerAt(const int x, const int y)
{
    AddMinePlacement(x, y);
}

void GameMapResources::LogResMap(void)
{
    printf("ResMap:\n");
    for (int x = 0; x < _w; x++)
    {
        for (int y = 0; y < _h; y++)
        {
            int val = _resourceValue[x + y * _w];
            char c = '.';
            if (val > 10)
                c = '#';
            else if (val > 0)
                c = '1' + val - 1;
            printf("%c", c);
        }
        printf("\n");
    }
}