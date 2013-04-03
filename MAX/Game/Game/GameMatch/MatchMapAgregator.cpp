//
//  MatchMapAgregator.cpp
//  MAX
//
//  Created by Anton Katekov on 01.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MatchMapAgregator.h"
#include "GameMap.h"
#include "GameUnit.h"
#include "GameMatchPlayer.h"
#include "GameFog.h"

MatchMapAgregator::MatchMapAgregator(GameMap* map)
:_map_w(map)
{}

MatchMapAgregator::~MatchMapAgregator()
{}