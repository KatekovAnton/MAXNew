//
//  MAXMapObject.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXMapObject.h"
#include "MAXContentMap.h"
#include "MAXContetnLoader.h"
#include "MapRenderObject.h"
#include "MAXMapMaterial.h"
#include "EngineMesh.h"

MAXMapObject::MAXMapObject(shared_ptr<MAXContentMap> map)
:PivotObject()
{
    _renderObject = new MapRenderObject(map);
    _material = new MAXMapMaterial(map);
    
    mapH = map->h;
    mapW = map->w;
    mapTexH = _material->texH;
    mapTexW = _material->texW;
    
    int vSize = sizeof(vertexStruct) * 4 * mapW * mapH;
    vertexStruct *points = (vertexStruct*)malloc(vSize);
    memset(points, 0, vSize);
    
    int iSize = mapW * mapH * 6 * sizeof(short);
    unsigned short* indices = (unsigned short *)malloc(iSize);
    memset(indices, 0, iSize);
    
    float minposx = -mapW/2.0;
    float minposy = mapH/2.0;
    
    float singleTileW = 1.0/mapTexW;
    float singleTileH = 1.0/mapTexH;
    
    float singleTileWD = singleTileW/128.0;
    float singleTileHD = singleTileH/128.0;
    
    for (int i = 0; i < mapH; i ++) {
        for (int j = 0; j < mapW; j ++) {
            
            int index = (i*mapW + j) * 4;
            int indexTL = index+0;
            int indexTR = index+1;
            int indexBL = index+2;
            int indexBR = index+3;
            
            short tileIndex = map->map[index/4];
            float _y = floor(tileIndex * singleTileW);
            float _x = tileIndex - _y / singleTileW;
            
            points[indexTL].position[0] = minposx + j;
            points[indexTL].position[1] = mapH - (minposy + i);
            points[indexTL].tcoord[0] = singleTileW * _x                    + singleTileWD;
            points[indexTL].tcoord[1] = singleTileH * _y                    + singleTileHD;
            
            
            
            points[indexBR].position[0] = minposx + j + 1;
            points[indexBR].position[1] = mapH - (minposy + i + 1);
            points[indexBR].tcoord[0] = singleTileW * _x + singleTileW      - singleTileWD;
            points[indexBR].tcoord[1] = singleTileH * _y + singleTileH      - singleTileHD;
            
            
            
            points[indexBL].position[0] = minposx + j;
            points[indexBL].position[1] = mapH - (minposy + i + 1);
            points[indexBL].tcoord[0] = singleTileW * _x                    + singleTileWD;
            points[indexBL].tcoord[1] = singleTileH * _y + singleTileH      - singleTileHD;
            
            
            
            points[indexTR].position[0] = minposx + j + 1;
            points[indexTR].position[1] = mapH - (minposy + i);
            points[indexTR].tcoord[0] = singleTileW * _x + singleTileW      - singleTileWD;
            points[indexTR].tcoord[1] = singleTileH * _y                    + singleTileHD;
            
            
            int indexIndex = (i*mapW + j) * 6;
            indices[indexIndex+0] = indexTL;
            indices[indexIndex+1] = indexBR;
            indices[indexIndex+2] = indexTR;
            indices[indexIndex+3] = indexBR;
            indices[indexIndex+4] = indexBL;
            indices[indexIndex+5] = indexTL;
            
        }
    }
    
    _renderObject->_mesh = shared_ptr<EngineMesh>(new EngineMesh(points, vSize, indices, iSize));
    free(indices);
    free(points);
}

MAXMapObject::~MAXMapObject()
{
    delete _renderObject;
    delete _material;
}

RenderObject * MAXMapObject::GetRenderAspect()
{
    return _renderObject;
}

Material * MAXMapObject::GetMaterial()
{
    return _material;
}

void MAXMapObject::Frame(double time)
{
    _material->DoFrame(time);
}


