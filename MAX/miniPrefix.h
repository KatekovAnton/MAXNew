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
    OBJECT_LEVEL_UNDERWATER   = 0,
    OBJECT_LEVEL_ONGROUND     = 1,
    OBJECT_LEVEL_OVERGROUND   = 2,
    OBJECT_LEVEL_BUILDING     = 3,
    OBJECT_LEVEL_UNITS        = 4,
    OBJECT_LEVEL_OVERUNITS    = 5,
    OBJECT_LEVEL_AIR          = 6,
    OBJECT_LEVEL_OVERAIR      = 7
} OBJECT_LEVEL;

typedef enum
{
    UNIT_MOVETYPE_GROUND = 0,
    UNIT_MOVETYPE_GROUNDCOAST = 1,
    UNIT_MOVETYPE_AMHIB = 2,
    UNIT_MOVETYPE_SEACOAST = 3,
    UNIT_MOVETYPE_SEA = 4,
    UNIT_MOVETYPE_AIR = 5,
    UNIT_MOVETYPE_SURVEYOR = 6 // like UNIT_MOVETYPE_AMHIB but special path cost for sea cells
} UNIT_MOVETYPE;

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

struct __Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
    bool IsNear(unsigned char a1, unsigned char a2) const
    {
        return MAX(a1, a2) - MIN(a1, a2)<3;
    }
    
    inline bool operator == (const __Color &color) const
    {
        return color.r == r && color.b == b && color.g == g && color.a == a; //IsNear(r, color.r) && IsNear(g, color.g) && IsNear(b, color.b) && IsNear(a, color.a);
    }
    
} typedef Color;
    
struct __MAXCoordinates
{
    int x;
    int y;
        
} typedef MAXCoordinates;
    
typedef enum
{
    GROUND_TYPE_GROUND = 0,
    GROUND_TYPE_WATER = 1,
    GROUND_TYPE_COAST = 2,
    GROUND_TYPE_UNPASSABLE = 3
} GROUND_TYPE;
    
typedef enum
{
    EXTENDED_GROUND_TYPE_GROUND = 0,
    EXTENDED_GROUND_TYPE_WATER = 1,
    EXTENDED_GROUND_TYPE_COAST = 2,
    EXTENDED_GROUND_TYPE_UNPASSABLE = 3,
    EXTENDED_GROUND_TYPE_ROAD = 4,
    EXTENDED_GROUND_TYPE_BRIDGE = 5
} EXTENDED_GROUND_TYPE;

typedef enum
{
    FIRE_TYPE_None = 0,
    FIRE_TYPE_Ground = 1,
    FIRE_TYPE_UnderWater = 2,
    FIRE_TYPE_Air = 3,
    FIRE_TYPE_All
}FIRE_TYPE;
    
typedef enum
{
    FOG_TYPE_SIMPLE = 0,
    FOG_TYPE_MINES = 1,
    FOG_TYPE_SEAMINES = 2,
    FOG_TYPE_UNDERWATER = 3,
    FOG_TYPE_INFILTRATOR = 4,
    FOG_TYPE_RESOURCES
}FOG_TYPE;
    
#endif
