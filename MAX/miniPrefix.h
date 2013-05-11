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

#define MAX_PLAYERS 32

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
    UNIT_PARAMETER_TYPE_MIN = 0,
    UNIT_PARAMETER_TYPE_SPEED = UNIT_PARAMETER_TYPE_MIN,
    UNIT_PARAMETER_TYPE_HEALTH,
    UNIT_PARAMETER_TYPE_ARMOR,
    UNIT_PARAMETER_TYPE_ATTACK,
    UNIT_PARAMETER_TYPE_SHOTS,
    UNIT_PARAMETER_TYPE_GAS,
    UNIT_PARAMETER_TYPE_AMMO,
    UNIT_PARAMETER_TYPE_CONSUMABLE_MAX = UNIT_PARAMETER_TYPE_AMMO,
    
    UNIT_PARAMETER_TYPE_RANGE,
    UNIT_PARAMETER_TYPE_SCAN,
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

    UNIT_PARAMETER_TYPE_MAX = UNIT_PARAMETER_TYPE_CARGO_SHIPS
   
    
} UNIT_PARAMETER_TYPE;

typedef enum
{
    UNIT_MENU_ACTION_ACTIVATE = 0,//??
    UNIT_MENU_ACTION_ALLOCATE,
    UNIT_MENU_ACTION_ATTACK,
    UNIT_MENU_ACTION_BUILD,
    UNIT_MENU_ACTION_BUYUPGRADES,
    UNIT_MENU_ACTION_CLEAR,
    UNIT_MENU_ACTION_DISABLE,
    UNIT_MENU_ACTION_DONE,
    UNIT_MENU_ACTION_ENTER,
    UNIT_MENU_ACTION_FOLLOW,//??
    UNIT_MENU_ACTION_LOAD,
    UNIT_MENU_ACTION_PLACE,//??
    UNIT_MENU_ACTION_RECHARGE,
    UNIT_MENU_ACTION_REMOVE,
    UNIT_MENU_ACTION_REPAIR,
    UNIT_MENU_ACTION_RELOAD,
    UNIT_MENU_ACTION_RESEARCH,
    UNIT_MENU_ACTION_SENTRY,
    UNIT_MENU_ACTION_START,
    UNIT_MENU_ACTION_STEAL,
    UNIT_MENU_ACTION_STOP,
    UNIT_MENU_ACTION_UPGRADE,
    UNIT_MENU_ACTION_WAIT,//??
    UNIT_MENU_ACTION_XFER,
    UNIT_MENU_ACTION_XFORM,//??
	UNIT_MENU_ACTION_INFO

} UNIT_MENU_ACTION;

typedef enum
{
    RESOURCE_LEVEL_POOR,
    RESOURCE_LEVEL_MEDIUM,
    RESOURCE_LEVEL_RICH,
    RESOURCE_LEVEL_COUNT
} RESOURCE_LEVEL;

typedef enum
{
    GIPARAMETER_COLOR_RED,
    GIPARAMETER_COLOR_YELLOW,
    GIPARAMETER_COLOR_GREEN
} GIPARAMETER_COLOR;

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
    
struct MAXRESTextureData
{
    Color* data;
    short w;
    short h;
        
    void FreeBuffer () {free(data); };        
};
    
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
    
typedef enum : unsigned char
{
    FOG_TYPE_MIN = 0,
    FOG_TYPE_SCAN = FOG_TYPE_MIN,
    FOG_TYPE_RESOURCES,
    FOG_TYPE_MINES,
    FOG_TYPE_UNDERWATER,
    FOG_TYPE_INFILTRATOR,
    FOG_TYPE_MAX
} FOG_TYPE;
    
typedef enum
{
    SOUND_TYPE_MIN = 0,
    SOUND_TYPE_END_OF_TURN,
    SOUND_TYPE_BUTTON_LARGE,
    SOUND_TYPE_BUTTON_AVERAGE,
    SOUND_TYPE_BUTTON_SMALL,
    SOUND_TYPE_ENEMY_DETECTED,
    SOUND_TYPE_MAX
}SOUND_TYPE;
    
#define GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER 1000
    
#endif
