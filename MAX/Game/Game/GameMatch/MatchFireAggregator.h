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
class GameMatch;
class GameUnit;

class MatchFireAggregator {
    
    GameMap *_map_w;
    GameMatch *_match_w;
    
    USimpleContainer<GameUnit*> **_unitsForCells_w;
    
public:
    
    USimpleContainer<GameUnit*> *UnitsForCell(const int x, const int y);
    int GetIndexForCoordinates(const int x, const int y) const;
    
    MatchFireAggregator(GameMap *map, GameMatch *match);
    ~MatchFireAggregator();
    
    int Width() const;
    int Height() const;
    void ClearAllData();
    
    void UnitDidEnterCell(GameUnit *unit, const CCPoint &cell);
    void UnitWillLeaveCell(GameUnit *unit, const CCPoint &cell);
    vector<GameUnit*> UnitsForAttackingUnitInCell(const int x, const int y, GameUnit* unit) const;

};

#endif /* defined(__MAX__MatchFireAggregator__) */
