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
#include "miniPrefix.h"

class GameMap;
class GameSettings;

class GameMapResources
{
    unsigned char*      _resourceValue;
    GameMap*            _map;
    GameSettings*       _settings;
    int                 _h;
    int                 _w;
    
    int GetIndexAt(const int x, const int y) const;
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
    unsigned char GetResourceValueAt(const int x, const int y) const;
    void LandPlayerAt(const int x, const int y);
    void LogResMap(void);
    
    GameMapResources(GameSettings* settings, GameMap* map);
    ~GameMapResources();
};

#endif /* defined(__MAX__GameMapResources__) */
