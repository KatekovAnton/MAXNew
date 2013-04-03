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

class GameMap;
class GameUnit;

class MatchMapAgregator {
    
    GameMap *_map_w;
  //  int *
    
public:
    
    MatchMapAgregator(GameMap* map);
    ~MatchMapAgregator();
    
};

#endif /* defined(__MAX__MatchMapAgregator__) */
