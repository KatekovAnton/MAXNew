//
//  MAXGrid.cpp
//  MAX
//
//  Created by  Developer on 29.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXGrid.h"
#include "MAXEngine.h"
#include "MAXCamera.h"
#include "MAXDrawPrimitives.h"


MAXGrid::MAXGrid()
{
    _startsW = NULL;
    _endsW = NULL;
    _startsH = NULL;
    _endsH = NULL;
    
    _maph = 0;
    _mapw = 0;
}

MAXGrid::~MAXGrid()
{
    freeArrays();
}

inline GLKVector2 GLKVector2MakeFromCCPoint(const CCPoint& point)
{
    return GLKVector2Make(point.x, point.y);
}

void MAXGrid::freeArrays()
{
    if (_startsW)
        free(_startsW);
    if (_startsH)
        free(_startsH);
    if (_endsW)
        free(_endsW);
    if (_endsH)
        free(_endsH);
}

void MAXGrid::SetMapSize(float mapw, float maph)
{
    bool needNewSize = mapw>_mapw || maph>_maph;
    _mapw = mapw;
    _maph = maph;
    UpdateInfo(needNewSize);
}

void MAXGrid::UpdateInfo(bool needNewSize)
{
    CCRect rect = engine->ScreenToWorldRect();

    if (rect.origin.x + rect.size.width > _mapw * 64) 
        rect.size.width = _mapw * 64 - rect.origin.x;
    if (rect.origin.y + rect.size.height > _maph * 64)
        rect.size.height = _maph * 64 - rect.origin.y;
    
    startx = (floorf(rect.origin.x / 64.0))*64;
    if(startx<0)
        startx = 0;
    
    starty = ((int)(rect.origin.y / 64))*64;
    if(starty<0)
        starty = 0;
    
    countx = rect.size.width/32.0 + 2;
    county = rect.size.height/32.0 + 2;
    
    if (countx > _mapw + 1)
        countx = _mapw + 1;
    if (county > _maph + 1)
        county = _maph + 1;
    
    step = 32.0 / engine->_camera->scale;
    
    
    if (needNewSize) {
        freeArrays();
        _startsW = (GLKVector2*)malloc((_mapw + 1) * sizeof(GLKVector2));
        _endsW = (GLKVector2*)malloc((_mapw + 1) * sizeof(GLKVector2));
        _startsH = (GLKVector2*)malloc((_maph + 1) * sizeof(GLKVector2));
        _endsH = (GLKVector2*)malloc((_maph + 1) * sizeof(GLKVector2));
    }
    GLKVector2 tmp;
    tmp.x = startx;
    tmp.y = starty;
    GLKVector2 start = engine->WorldCoordinatesToScreenCocosV(tmp);
    GLKVector2 end = GLKVector2Make(start.x, 0);
    for (int i = 0; i < countx; i++) {
        _startsW[i] = start;
        _endsW[i] = end;
        start.x += step;
        end.x += step;
    }
    tmp.x = startx;
    tmp.y = starty;
    
    
    
    start = engine->WorldCoordinatesToScreenCocosV(tmp);
    end = GLKVector2Make(engine->displayw, start.y);
    for (int i = 0; i < county; i++) {
        _startsH[i] = start;
        _endsH[i] = end;
        start.y -= step;
        end.y -= step;
    }
}

void MAXGrid::DrawGrid()
{
    MAXDrawPrimitives::SharedDrawPrimitives()->Begin();
    MAXDrawPrimitives::SharedDrawPrimitives()->BindColor(GLKVector4Make(14.0/255.0, 58.0/255.0, 0, 0.7));
    //ccDrawColor4F(14.0/255.0, 58.0/255.0, 0, 0.7);
    for (int i = 0; i < countx; i++)
    {
        MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_startsW[i], _endsW[i]);
       // ccDrawLine(_startsW[i], _endsW[i]);
    }
    
    for (int i = 0; i < county; i++)
    {
        MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_startsH[i], _endsH[i]);
      //  ccDrawLine(_startsH[i], _endsH[i]);
    }
    
}