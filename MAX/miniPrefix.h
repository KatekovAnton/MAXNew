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
    
    
    //storage base
    UNIT_PARAMETER_TYPE_FUEL_BASE,
    UNIT_PARAMETER_TYPE_GOLD_BASE,
    UNIT_PARAMETER_TYPE_MATERIAL_BASE,
    UNIT_PARAMETER_TYPE_PEOPLE_BASE,
    UNIT_PARAMETER_TYPE_ENERGY_BASE,
    
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

typedef enum : int
{
    UNIT_SOUND_SHOT,
    UNIT_SOUND_BLAST,
    
    UNIT_SOUND_ENGINE,
    UNIT_SOUND_ENGINE_START,
    UNIT_SOUND_ENGINE_STOP,
    UNIT_SOUND_ENGINE_WATER,
    UNIT_SOUND_ENGINE_START_WATER,
    UNIT_SOUND_ENGINE_STOP_WATER,

    UNIT_SOUND_START,
    UNIT_SOUND_STOP,
    UNIT_SOUND_WORK,
    UNIT_SOUND_BUILD
} UNIT_SOUND;

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
    
struct __GameMatchPlayerInfo {
        
    int             _clan;
    unsigned int    _playerId;
    string          _name;
    Color           _color;
} typedef GameMatchPlayerInfo;
    
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
    SOUND_TYPE_START_OF_TURN,
    SOUND_TYPE_BUTTON_LARGE,
    SOUND_TYPE_BUTTON_AVERAGE,
    SOUND_TYPE_BUTTON_SMALL,
    SOUND_TYPE_ENEMY_DETECTED,
    SOUND_TYPE_ENEMY_FIRING_ON_UNIT, //f229.wav f230.wav f231.wav f232.wav f255.wav f256.wav
    SOUND_TYPE_UNIT_FIRING,//f250.wav f251.wav
    SOUND_TYPE_READY,
    SOUND_TYPE_INFILTRATOR_FAIL,
    SOUND_TYPE_GAME_SAVED,
    SOUND_TYPE_FUEL_STORAGE_FULL,//f057.wav
    SOUND_TYPE_RAWM_STORAGE_FULL,//f061.wav
    SOUND_TYPE_GOLD_STORAGE_FULL,//f066.wav
    SOUND_TYPE_MOVE_TAPE_TO_BULD,//f050.wav
    SOUND_TYPE_RELOADED,//f085.wav f089.wav
    SOUND_TYPE_RESEARCH_COMPLETE, //f093.wav
    SOUND_TYPE_NO_PATH_TO_DESTINATION, //f094.wav
    SOUND_TYPE_PATH_BLOCKED,//f095.wav
    SOUND_TYPE_ATTEMPT_FAILED,//f007.wav
    SOUND_TYPE_AMMO_LOW,//f138.wav f270.wav f271.wav
    SOUND_TYPE_AMMO_NONE,//f142.wav
    SOUND_TYPE_MOVEMENT_EXHAUSTED,//f145.wav
    SOUND_TYPE_STATUS_YELLOW,//f150.wav f151.wav
    SOUND_TYPE_STATUS_RED,//f154.wav f155.wav
    SOUND_TYPE_ON_SENTRY,//f158.wav
    SOUND_TYPE_CONSTRUCTION_COMPLETE,//f162.wav f165.wav
    SOUND_TYPE_READY_FOR_OPERATION,//f163.wav
    SOUND_TYPE_COMPLETE_AND_READY,//f164.wav
    SOUND_TYPE_UNIT_COMPLETED,//f166.wav  f169.wav  f216.wav f217.wav
    SOUND_TYPE_CLEARING_AREA,//f171.wav
    SOUND_TYPE_STARTGAME_SELECT_SIDE,//f176.wav f177.wav
    SOUND_TYPE_LAYING_MINES,//f181.wav f182.wav
    SOUND_TYPE_REMOVING_MINES,// f186.wav f187.wav
    SOUND_TYPE_SURVEYING,//f191.wav f192.wav
    SOUND_TYPE_ATTACKING,//f196.wav f198.wav 
    SOUND_TYPE_SUBMARINE_DETECTED,//f201.wav
    SOUND_TYPE_NEW_UNITS_AVAILABLE_THIS_TURN,//f206.wav f207.wav f215.wav
    SOUND_TYPE_UNITS_REPAIRED,//f210.wav f211.wav
    SOUND_TYPE_UNIT_REPAIRED,// f219.wav f220.wav
    SOUND_TYPE_TRANSFER_COMPLETED,//f224.wav
    SOUND_TYPE_UNIT_DESTROYED,//f234.wav f236.wav
    SOUND_TYPE_UNIT_CAPTURED,//f239.wav  f242.wav
    SOUND_TYPE_UNIT_HAS_BEEN_SEASED,//f243.wav
    SOUND_TYPE_UNIT_DISABLED,//f244.wav
    SOUND_TYPE_UNIT_MUST_BE_REPAIRED,//f247.wav f265.wav
    SOUND_TYPE_UNIT_NOT_FUNCTIONATING,//f249.wav
    SOUND_TYPE_STARTGAME_SELECT_LAND_POSITION,//f278.wav
    SOUND_TYPE_MISSION_SUCCESSFULL,//f283.wav
    SOUND_TYPE_BUILDING,//f284.wav
    SOUND_TYPE_CANT_START,//mstat18.wav
    SOUND_TYPE_MAX = SOUND_TYPE_BUILDING,
    SOUND_TYPE_NONE
    
}SOUND_TYPE;
    
typedef enum
{
    EXPLODE_SOUND_TYPE_NONE,
    EXPLODE_SOUND_TYPE_BLOCK,//block16.wav
    
    EXPLODE_SOUND_TYPE_LAND_SMALL_UNIT = 0,//explmed.wav explsmal.wav
    EXPLODE_SOUND_TYPE_LAND_SMALL_BUILD,//bldexplg.wav expllrge.wav
    EXPLODE_SOUND_TYPE_LAND_LARGE_BUILD,//expllrge.wav  expbuld5.wav expbuld6.wav explbld1.wav explbld2.wav
    
    EXPLODE_SOUND_TYPE_SEA_SMALL_UNIT,//boatexp1.wav  
    EXPLODE_SOUND_TYPE_SEA_SMALL_BUILD,//eplowet1.wav eplowet2.wav 
    EXPLODE_SOUND_TYPE_SEA_LARGE_BUILD,//cbldexp1.wav cbldexp2.wav
    
    EXPLODE_SOUND_TYPE_UNDERWATER_UNIT,//sub14.wav sub16.wav
    EXPLODE_SOUND_TYPE_MINE,//cmine16.wav
    EXPLODE_SOUND_TYPE_DIRT,//expsdirt.wav
    EXPLODE_SOUND_TYPE_AIR,//explsmal.wav
    EXPLODE_SOUND_TYPE_INFANTRY,//infan15 infan16
    EXPLODE_SOUND_TYPE_INFILTRATOR,//infil15 infil16
}EXPLODE_SOUND_TYPE;
    
    typedef enum
    {
        EFFECT_TYPE_BLAST,
        EFFECT_TYPE_BULLET,
        EFFECT_TYPE_SECONDARY,
        EFFECT_TYPE_TRASH,
        EFFECT_TYPE_BUILDING_BASE
    } EFFECT_TYPE;
    
    typedef enum
    {
        BLAST_TYPE_GROUND,
        BLAST_TYPE_SEA,
        BLAST_TYPE_AIR,
        BLAST_TYPE_BUILDING,
        BLAST_TYPE_DAMAGEEFFECT,
        BLAST_TYPE_NONE
    } BLAST_TYPE;
    
    typedef enum
    {
        BULLET_TYPE_ROCKET,
        BULLET_TYPE_TORPEDO,
        BULLET_TYPE_PLASMA,
        BULLET_TYPE_NONE
    } BULLET_TYPE;
    
    typedef enum
    {
        SECONDARY_TYPE_SMOKE,
        SECONDARY_TYPE_RIBBLES,
        SECONDARY_TYPE_NONE
    } SECONDARY_TYPE;
    
    typedef enum
    {
        TRASH_TYPE_SMALL,
        TRASH_TYPE_LARGE
    } TRASH_TYPE;
    
    typedef enum
    {
        BUILDING_BASE_TYPE_LARGE,
        BUILDING_BASE_TYPE_SMALL,
        BUILDING_BASE_TYPE_PROGRESS_SMALL,
        BUILDING_BASE_TYPE_PROGRESS_LARGE,
        BUILDING_BASE_TYPE_PROGRESS_SEA_SMALL,
        BUILDING_BASE_TYPE_PROGRESS_SEA_LARGE
    } BUILDING_BASE_TYPE;
    
#define GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER 1000
    
#endif
