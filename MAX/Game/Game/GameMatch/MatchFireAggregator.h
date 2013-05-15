//
//  MatchFireAggregator.h
//  MAX
//
//  Created by Anton Katekov on 15.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MatchFireAggregator__
#define __MAX__MatchFireAggregator__

#include <iostream>
#include "miniPrefix.h"

class GameMap;
class GameUnit;

class MatchFireAggregator {
    
    GameMap *_map_w;
    
    USimpleContainer<GameUnit*> **_unitsInCells_w;
    
public:
    
    MatchFireAggregator(GameMap *map);
    ~MatchFireAggregator();
    
};

#endif /* defined(__MAX__MatchFireAggregator__) */
