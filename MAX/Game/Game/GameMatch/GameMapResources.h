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

typedef enum
{
    RESOURCE_TYPE_NONE,
    RESOURCE_TYPE_RAW,
    RESOURCE_TYPE_FUEL,
    RESOURCE_TYPE_GOLD,
    RESOURCE_TYPE_COUNT
} RESOURCE_TYPE;

typedef enum
{
    RESOURCE_LEVEL_POOR,
    RESOURCE_LEVEL_MEDIUM,
    RESOURCE_LEVEL_RICH,
    RESOURCE_LEVEL_COUNT
} RESOURCE_LEVEL;

class GameMapResources
{
    unsigned char* _resourceValue;
public:
    void GenerateInitialResources(const RESOURCE_LEVEL resLevels[RESOURCE_TYPE_COUNT]) const;
    RESOURCE_TYPE GetResourceTypeAt(const int x, const int y) const;
    
    GameMapResources(GameMap* gameMap);
    ~GameMapResources();
};

#endif /* defined(__MAX__GameMapResources__) */
