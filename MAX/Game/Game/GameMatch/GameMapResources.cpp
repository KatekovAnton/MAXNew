//
//  GameMapResources.cpp
//  MAX
//
//  Created by Alexey Malyshko on 3/25/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMapResources.h"
#include "GameMap.h"

GameMapResources::GameMapResources(GameMap* gameMap)
{
    _resourceValue = (unsigned char*)malloc(gameMap->_w * gameMap->_h);
}

GameMapResources::~GameMapResources()
{
    free(_resourceValue);
}

RESOURCE_TYPE GameMapResources::GetResourceTypeAt(const int x, const int y) const
{
    RESOURCE_TYPE result;
    if ((y % 2) == 0)
    {
        if ((x % 2) == 0)
        {
            result = RESOURCE_TYPE_FUEL;
        }
        else
        {
            result = RESOURCE_TYPE_NONE;
        }
    }
    else
    {
        if ((x % 2) == 0)
        {
            result = RESOURCE_TYPE_GOLD;
        }
        else
        {
            result = RESOURCE_TYPE_RAW;
        }
    }
    return result;
}

typedef enum
{
    RES_MODE_MINE = 0,
    RES_MODE_CONCENTRATE,
    RES_MODE_NORMAL,
    RES_MODE_DIFFUSION,
    RES_MODE_COUNT
} RES_MODE_TYPE;

typedef enum
{
    P_MIN = 0,
    P_MAX,
    P_COUNT
} P_KIND;


static const int RES_DATA[RESOURCE_TYPE_COUNT][RESOURCE_LEVEL_COUNT][RES_MODE_COUNT][P_COUNT] =
{
    //none
    {   // MINE    CONC    NORM    DIFF
        {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}}, //R_POOR
        {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}}, //R_MEDIUM
        {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}}  //R_RICH
    },
    //material
    {   // MINE    CONC    NORM    DIFF
        {{10,10},{ 8,12},{ 0, 3},{ 8, 0}}, //R_POOR
        {{12,12},{13,16},{ 1, 5},{ 6, 0}}, //R_MEDIUM
        {{14,14},{16,16},{ 1, 5},{ 4, 0}}  //R_RICH
    },
    //fuel
    {   // MINE    CONC    NORM    DIFF
        {{ 7, 7},{ 8,12},{ 1, 2},{ 8, 0}}, //R_POOR
        {{ 8, 8},{12,16},{ 2, 3},{ 6, 0}}, //R_MEDIUM
        {{ 9, 9},{16,16},{ 2, 4},{ 4, 0}}  //R_RICH
    },
    //gold
    {   // MINE    CONC    NORM    DIFF
        {{ 0, 0},{ 5, 9},{ 0, 0},{12, 0}}, //R_POOR
        {{ 0, 0},{ 8,12},{ 0, 0},{10, 0}}, //R_MEDIUM
        {{ 1, 1},{12,16},{ 0, 1},{ 8, 0}}  //R_RICH
    }
};

int getAmount(RESOURCE_TYPE type, RES_MODE_TYPE mode)
{
    //    rand()%max
    //int valMin = RES_DATA[type][]
}

void GameMapResources::GenerateInitialResources(const RESOURCE_LEVEL resLevels[RESOURCE_TYPE_COUNT]) const
{
    
}


/*

 //############################################################################//
 function get_amt(typ:integer;mode:integer):integer;
 var r1,r2:integer;
 begin
 r1:=R_DATA[typ,mg_game.rules.res_levels[typ],mode,P_MIN];
 r2:=R_DATA[typ,mg_game.rules.res_levels[typ],mode,P_MAX];
 result:=r1+random(r2-r1+1);
 end;
 //############################################################################//
 function get_dif(typ:integer):integer;
 begin
 result:=R_DATA[typ,mg_game.rules.res_levels[typ],M_DIFFUSION,P_MIN];
 end;
 //############################################################################//
 function getrescell(x,y:integer;out r:presrec):boolean;
 begin
 result:=false;
 if(x<0)or(y<0)or(x>=mg_game.mapx)or(y>=mg_game.mapy)then exit;
 r:=@mg_game.resmap[x+y*mg_game.mapx];
 result:=true;
 end;
 //############################################################################//
 function getrescell_amt(x,y:integer):integer;
 var r:presrec;
 begin
 if getrescell(x,y,r) then result:=r.amt
 else result:=0;
 end;
 //############################################################################//
 //Resource distribution
 //Needs fixing resource separation and diffusion
 procedure initial_resource_placement(x,y:integer;fix:boolean);
 var r:presrec;
 i,j,typ,amt:integer;
 begin
 if(x<0)or(y<0)or(x>=mg_game.mapx)or(y>=mg_game.mapy)then exit;
 
 if fix then begin
 //find material place (center of new resource placement)
 for i:=0 to 1 do for j:=0 to 1 do if celltp(x+i,y+j)=1 then begin
 initial_resource_placement(x+i,y+j,false);
 break;
 end;
 
 for i:=0 to 1 do for j:=0 to 1 do if getrescell(x+i,y+j,r) then begin
 r.typ:=celltp(x+i,y+j);
 amt:=get_amt(r.typ,M_MINE);
 if amt>0 then r.amt:=amt-1 else r.typ:=0;
 end;
 end else begin
 if getrescell(x,y,r) then begin
 typ:=r.typ;
 r.typ:=celltp(x,y);
 amt:=get_amt(r.typ,M_CONCENTRATE);
 if typ<>0 then amt:=round((r.amt+1+amt)/2);
 if amt>0 then r.amt:=amt-1 else r.typ:=0;
 end;
 
 for i:=-1 to 1 do for j:=-1 to 1 do if (i<>0)or(j<>0) then if getrescell(x+i,y+j,r) then begin
 typ:=r.typ;
 r.typ:=celltp(x+i,y+j);
 amt:=get_amt(r.typ,M_NORMAL);
 if typ<>0 then amt:=round((r.amt+1+amt)/2);
 if amt>0 then r.amt:=amt-1 else r.typ:=0;
 end;
 end;
 end;
 //############################################################################//
 procedure clear_resources;
 var i:integer;
 begin try
 setlength(mg_game.resmap,mg_game.mapx*mg_game.mapy);
 for i:=0 to mg_game.mapx*mg_game.mapy-1 do begin
 mg_game.resmap[i].amt:=0;
 mg_game.resmap[i].typ:=0;
 end;
 except stderr('Resman','clear_resources');end;
 end;
 //############################################################################//
 procedure set_initial_resources;
 var i,tp,resmax,x,y,x1,y1,d,okmax:integer;
 is_ok:boolean;
 rescount:array[1..3] of integer;
 begin try
 clear_resources;
 resmax:=round(mg_game.rules.resset*((mg_game.mapx*mg_game.mapy)/(112*112)));
 rescount[2]:=round(resmax*0.40);            //40% of fuel
 rescount[3]:=round(resmax*0.15);            //15% of gold
 rescount[1]:=resmax-rescount[2]-rescount[3];//45% of metal
 
 for i:=1 to resmax do begin
 for tp:=1 to 3 do begin
 if rescount[tp]>0 then begin
 okmax:=60;
 repeat
 //fuel cell by default
 x:=random(mg_game.mapx div 2)*2;
 y:=random(mg_game.mapy div 2)*2;
 case tp of
 1:begin x:=x+1;y:=y+1;end;//metal
 2:begin end;//fuel
 3:begin y:=y+1;end;//gold
 end;
 is_ok:=inrm(x,y);
 //check resource placement for obstacles
 if is_ok then for x1:=x-1 to x+1 do for y1:=y-1 to y+1 do if inrm(x1,y1) and(tpassm(x1,y1)=P_OBSTACLE) then is_ok:=false;
 //check around cells for same res
 if is_ok then begin
 d:=get_dif(tp);
 if okmax<20 then d:=d div 2;
 if okmax<10 then d:=d div 2;
 if d=0 then d:=1;
 if okmax>0 then for x1:=-d to +d do for y1:=-d to +d do if inrm(x+x1*2,y+y1*2)and(getrescell_amt(x+x1*2,y+y1*2)>=7)then is_ok:=false;
 dec(okmax);
 end;
 until(is_ok)and((y mod 2<>0)or(x mod 2=0));
 rescount[tp]:=rescount[tp]-1;
 initial_resource_placement(x,y,false);
 end;
 end;
 end;
 except stderr('Resman','set_initial_resources');end;
 end;
 */

