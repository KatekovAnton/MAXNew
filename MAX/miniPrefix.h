//
//  miniPrefix.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_miniPrefix_h
#define MAX_miniPrefix_h

#include <vector>
#include <string>
#include <list>
#include <stack>

#include "Geometry.h"
#include "UContainer.h"
#include "USimpleContainer.h"
#include "IdGenerator.h"
#include "Sys.h"
#include "cocos2d.h"
#include "clist.h"

using namespace cocos2d;
using namespace std;
using namespace Utils;

const float interfaceAnimationTime = 0.2;

typedef enum
{
    //unit parameters
    UNIT_PARAMETER_TYPE_SPEED,
    UNIT_PARAMETER_TYPE_HEALTH,
    UNIT_PARAMETER_TYPE_ARMOR,
    UNIT_PARAMETER_TYPE_ATTACK,
    UNIT_PARAMETER_TYPE_SHOTS,
    UNIT_PARAMETER_TYPE_RANGE,
    UNIT_PARAMETER_TYPE_SCAN,
    UNIT_PARAMETER_TYPE_AMMO,
    UNIT_PARAMETER_TYPE_COST,
    
    //storage
    UNIT_PARAMETER_TYPE_FUEL,
    UNIT_PARAMETER_TYPE_GOLD,
    UNIT_PARAMETER_TYPE_MATERIAL,
    UNIT_PARAMETER_TYPE_PEOPLE,
    UNIT_PARAMETER_TYPE_ENERGY,
    
    //caro
    UNIT_PARAMETER_TYPE_CARGO_UNITS,
    UNIT_PARAMETER_TYPE_CARGO_PLANES,
    UNIT_PARAMETER_TYPE_CARGO_SHIPS,
    
} UNIT_PARAMETER_TYPE;

typedef enum
{
    RESOURCE_LEVEL_POOR,
    RESOURCE_LEVEL_MEDIUM,
    RESOURCE_LEVEL_RICH,
    RESOURCE_LEVEL_COUNT
} RESOURCE_LEVEL;

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

#endif
