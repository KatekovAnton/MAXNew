//
//  MatchGarbageMap.h
//  MAX
//
//  Created by  Developer on 03.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MatchGarbageMap__
#define __MAX__MatchGarbageMap__

#include <iostream>
#include "miniPrefix.h"

class GameMap;
class MatchMapAgregator;
class GameEffect;

class GarbageElement {
    
public:
    
    int _x;
    int _y;
    int _size;
    int _rawMat;
    
    GameEffect *_effect_w;
    
};

class MatchGarbageMap {
    
    GameMap *_map_w;
    MatchMapAgregator *_map_agregator;
    
    int size;
    USimpleContainer<GameEffect*> *_effects;
    USimpleContainer<GarbageElement*> **_garbageInCells;
    
public:
    
    int GetIndexForCoordinates(const int x, const int y) const;
    
    MatchGarbageMap(GameMap *map, MatchMapAgregator *agregator);
    ~MatchGarbageMap();
    
    vector<GameEffect*> EffectsInCell(const int x, const int y);
    const GarbageElement *GarbageInCell(const int x, const int y);
    bool ContainGarbageInCell(const int x, const int y);
    
    void AddGarbageToCell(const int x, const int y, const int size, const int rawMat);
    bool ModifyGarbageInCell(const GarbageElement* element, const int deltaRawMat);
    

};

#endif /* defined(__MAX__MatchGarbageMap__) */
