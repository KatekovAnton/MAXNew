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
#include "MAXREsourceMapMaterial.h"
#include "MAXContentLoader.h"

int MAXResourceMapRenderer::GetIndexForCoordinates(const int x, const int y) const
{
    return y * _mapW + x;
}

int MAXResourceMapRenderer::GetTileIndexForResourceTypeAndAmount(const RESOURCE_TYPE type, const unsigned char amount) const
{
    if (type == RESOURCE_TYPE_NONE || amount == 0) 
        return 0;
    
    int stride = 1;
    
    if (type == RESOURCE_TYPE_GOLD)
        stride += 16;
    if (type == RESOURCE_TYPE_RAW)
        stride += 32;
        
    return stride + (int)amount - 1;
}

MAXResourceMapRenderer::MAXResourceMapRenderer(const int mapW, const int mapH)
:_mesh(NULL), _material(new MAXREsourceMapMaterial()), _cells(new bool[mapH * mapW]), _mapW(mapW), _mapH(mapH)
{
    memset(_cells, 0, mapH * mapW);
    EngineTiledDynamicMeshTextureInfo info;
    info.tileCountH = 2;
    info.tileCountW = 32;
    _mesh = new EngineTiledDynamicMesh(info, mapW, mapH);
    
    
}

MAXResourceMapRenderer::~MAXResourceMapRenderer()
{
    delete _mesh;
    delete _material;
    delete[] _cells;
}

void MAXResourceMapRenderer::AddCellToScan(const int x, const int y, const RESOURCE_TYPE type, const unsigned char amount)
{
    if (_cells[GetIndexForCoordinates(x, y)]) 
        return;
    _cells[GetIndexForCoordinates(x, y)] = true;
    _mesh->AddPolygon(x, y, GetTileIndexForResourceTypeAndAmount(type, amount), GetIndexForCoordinates(x, y));
}

void MAXResourceMapRenderer::Draw(Shader *shader)
{
    _material->ApplyLod(0, shader);
    _mesh->Draw();
}

void MAXResourceMapRenderer::Clear()
{
    memset(_cells, 0, _mapH * _mapW * sizeof(bool));
    _mesh->Clear();
}

