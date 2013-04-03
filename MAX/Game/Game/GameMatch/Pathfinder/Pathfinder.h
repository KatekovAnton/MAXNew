//
//  Pathfinder.h
//  MAX
//
//  Created by Alexey Malyshko on 3/27/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Pathfinder__
#define __MAX__Pathfinder__

#include <iostream>
#include "MAXObjectConfig.h"

typedef struct
{
	int distance;               // number of cell in the path
	int cost;                   // path cost
	unsigned char direction;    // direction come from
} PFPathMapCell;

class	PFWave;
class   PFWaveCell;
class   GameMap;

class Pathfinder
{
	PFPathMapCell*	pathMap;
    int             w;
    int             h;
	PFWave*			oldWave;
	PFWave*			newWave;
    int _fromX, _fromY;
    
    GameMap*        map;
    UNIT_MOVETYPE   moveType;
    
    void PreparePathMap(void);
    int GetCost(const int x, const int y, unsigned char direction);
    void TestNeighbours(void);
    void ExchangeWaves(void);
    int GetIndexAt(const int x, const int y);
    void FillPathMap(const int fromX, const int fromY, const int toX, const int toY);
public:
    Pathfinder(GameMap* _map);
    ~Pathfinder();
    
    std::vector<PFWaveCell*> FindPath(const int fromX, const int fromY, const int toX, const int toY, UNIT_MOVETYPE unitMoveType);

    void MakePathMap(const int fromX, const int fromY, UNIT_MOVETYPE unitMoveType); // Fill path map. Used if need to get cost for whole map
    std::vector<PFWaveCell*> FindPathOnMap(const int toX, const int toY); // MakePathMap should be called first
    int GetCostAt(const int x, const int y); // MakePathMap should be called first
    void DumpMap(void); // MakePathMap should be called first
};

#endif /* defined(__MAX__Pathfinder__) */