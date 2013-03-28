//
//  MAXResourceMapRenderer.cpp
//  MAX
//
//  Created by  Developer on 26.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXResourceMapRenderer.h"
#include "EngineTiledDynamicMesh.h"
#include "Texture.h"
#include "MAXContentLoader.h"

int MAXResourceMapRenderer::GetIndexForCoordinates(int x, int y) const
{
    return y * _mapW + x;
}

int MAXResourceMapRenderer::GetTileIndexForResourceTypeAndAmount(RESOURCE_TYPE type, unsigned char amount) const
{
    if (type == RESOURCE_TYPE_NONE || amount == 0) 
        return 0;
    
    int stride = 1;
    
    if (type == RESOURCE_TYPE_GOLD)
        stride += 16;
    if (type == RESOURCE_TYPE_RAW)
        stride += 32;
        
    return stride + amount - 1;
}

MAXResourceMapRenderer::MAXResourceMapRenderer(int mapW, int mapH)
:_mesh(NULL), _texture(NULL), _cells(new bool[mapH * mapW]), _mapW(mapW), _mapH(mapH)
{
    memset(_cells, 0, mapH * mapW);
    _texture = MAXSCL->resourceTiles;
    EngineTiledDynamicMeshTextureInfo info;
    info.tileCountH = 2;
    info.tileCountW = 32;
    _mesh = new EngineTiledDynamicMesh(info);
}

MAXResourceMapRenderer::~MAXResourceMapRenderer()
{
    delete _mesh;
    delete _texture;
    delete[] _cells;
    delete [] _tileInformation;
}

void MAXResourceMapRenderer::AddCellToScan(int x, int y, RESOURCE_TYPE type, unsigned char amount)
{
    _cells[GetIndexForCoordinates(x, y)] = true;
    _mesh->AddPolygon(x, y, GetTileIndexForResourceTypeAndAmount(type, amount));
}

void MAXResourceMapRenderer::Draw()
{
    
}

