//
//  MatchMapAgregator.h
//  MAX
//
//  Created by Anton Katekov on 01.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MatchMapAgregator__
#define __MAX__MatchMapAgregator__

#include <iostream>
#include "miniPrefix.h"

using namespace Utils;

class GameMap;
class GameUnit;
class GameMatchPlayer;

class MatchMapAgregator {
    
    GameMap *_map_w;
    
    USimpleContainer<GameUnit*> **_unitsInCells_w;
    EXTENDED_GROUND_TYPE* _mapBuffer;
    EXTENDED_GROUND_TYPE* _mapAirBuffer;
    EXTENDED_GROUND_TYPE* _mapOutputBuffer;
    
public:
    USimpleContainer<GameUnit*> *UnitsInCell(const int x, const int y) const;
    int GetIndexForCoordinates(const int x, const int y) const;
    EXTENDED_GROUND_TYPE GroundTypeAtXY(const int x, const int y) const;
    
    MatchMapAgregator(GameMap* map);
    ~MatchMapAgregator();
    
    int Width();
    int Height();
    
    //unit events
    void RemoveUnitFromCell(GameUnit *unit, const int x, const int y);
    void AddUnitToCell(GameUnit *unit, const int x, const int y);
    GameUnit* GetUnitInPosition(const int x, const int y);
    bool IsGroundUnitInPosition(const int x, const int y);
    bool IsAirUnitInPosition(const int x, const int y);
    EXTENDED_GROUND_TYPE *CalculateFieldForPlayer(GameMatchPlayer *player, GameUnit *unit);
};

#endif /* defined(__MAX__MatchMapAgregator__) */
