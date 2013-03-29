//
//  EngineTiledDynamicMesh.cpp
//  MAX
//
//  Created by  Developer on 26.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "EngineTiledDynamicMesh.h"
#include <math.h>

const int vertexSize = sizeof(vertexStruct);
const size_t vertexPositionOffset = offsetof(vertexStruct,position);
const size_t vertexNormalOffset = offsetof(vertexStruct,normal);
const size_t vertexTcoordOffset = offsetof(vertexStruct,tcoord);

EngineTiledDynamicMesh::EngineTiledDynamicMesh(EngineTiledDynamicMeshTextureInfo textureInfo, int mapW, int mapH)
:_textureInfo(textureInfo), _vertices(new USimpleContainer<Polygon>(mapH*mapW)), _indices(new USimpleContainer<int>(100)), _mapW(mapW), _mapH(mapH)
{
    
}

EngineTiledDynamicMesh::~EngineTiledDynamicMesh()
{}

void EngineTiledDynamicMesh::AddPolygon(int x, int y, int tileIndex)
{
    Polygon poly;
    
    float minposx = -_mapW/2.0;
    float minposy = _mapH/2.0;
    
    float singleTileW = 1.0/_textureInfo.tileCountW;
    float singleTileH = 1.0/_textureInfo.tileCountH;
    
    
    float _y = floor(tileIndex * singleTileW);
    float _x = tileIndex - _y / singleTileW;
    
    vertexStruct vertexTL;
    vertexStruct vertexBR;
    vertexStruct vertexTR;
    vertexStruct vertexBL;
    
    
    
    vertexTL.position[0] = minposx + x;
    vertexTL.position[1] = _mapH - (minposy + y);
    vertexTL.tcoord[0] = singleTileW * _x;
    vertexTL.tcoord[1] = singleTileH * _y;
    

    vertexBR.position[0] = minposx + x + 1;
    vertexBR.position[1] = (_mapH - (minposy + y + 1));
    vertexBR.tcoord[0] = singleTileW * _x + singleTileW;
    vertexBR.tcoord[1] = singleTileH * _y + singleTileH;
    
    
    vertexBL.position[0] = minposx + x;
    vertexBL.position[1] = (_mapH - (minposy + y + 1));
    vertexBL.tcoord[0] = singleTileW * _x;
    vertexBL.tcoord[1] = singleTileH * _y + singleTileH;
    
    
    vertexTR.position[0] = minposx + x + 1;
    vertexTR.position[1] = _mapH - (minposy + y);
    vertexTR.tcoord[0] = singleTileW * _x + singleTileW;
    vertexTR.tcoord[1] = singleTileH * _y;
    
    
    
    poly.vertexTL = vertexTL;
    poly.vertexBR = vertexBR;
    poly.vertexTR = vertexTR;
    poly.vertexBR1 = vertexBR;
    poly.vertexBL = vertexBL;
    poly.vertexTL1 = vertexTL;
    
    _vertices->addObject(poly);
}

void EngineTiledDynamicMesh::Draw()
{
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)((char*)_vertices->GetArrayPointer() + vertexPositionOffset));
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)((char*)_vertices->GetArrayPointer() + vertexNormalOffset));
    glEnableVertexAttribArray(ATTRIB_TCOORD);
    glVertexAttribPointer(ATTRIB_TCOORD, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)((char*)_vertices->GetArrayPointer() + vertexTcoordOffset));
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
}