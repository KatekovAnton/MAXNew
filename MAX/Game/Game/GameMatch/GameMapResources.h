//
//  GameMapResources.h
//  MAX
//
//  Created by Alexey Malyshko on 3/25/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameMapResources__
#define __MAX__GameMapResources__

#include <iostream>

class GameMap;
class GameSettings;

typedef enum
{
    RESOURCE_TYPE_NONE = -1,
    RESOURCE_TYPE_RAW  = 0,
    RESOURCE_TYPE_FUEL = 1,
    RESOURCE_TYPE_GOLD = 2,
    RESOURCE_TYPE_COUNT
} RESOURCE_TYPE;

typedef enum
{
    RES_MODE_MINE = 0,
    RES_MODE_CONCENTRATE,
    RES_MODE_NORMAL,
    RES_MODE_DIFFUSION,
    RES_MODE_COUNT
} RES_MODE_TYPE;

class GameMapResources
{
    unsigned char*      _resourceValue;
    GameMap*            _map;
    GameSettings*       _settings;
    int                 _h;
    int                 _w;
    
    int GetIndexAt(const int x, const int y);
    void SetAmount(const int x, const int y, const int amount);
    void MergeAmount(const int x, const int y, const int amount);
    unsigned char GetAmount(RESOURCE_TYPE type, RES_MODE_TYPE mode);
    unsigned int GetDiff(RESOURCE_TYPE type);
    void AddMinePlacement(const int x, const int y);
    void AddResourcePlacement(const int x, const int y);
    void ClearResources(void);
public:
    void GenerateInitialResources();
    RESOURCE_TYPE GetResourceTypeAt(const int x, const int y) const;
    unsigned char GetResourceValueAt(const int x, const int y);
    void LandPlayerAt(const int x, const int y);
    void LogResMap(void);
    
    GameMapResources(GameSettings* settings, GameMap* map);
    ~GameMapResources();
};

#endif /* defined(__MAX__GameMapResources__) */
