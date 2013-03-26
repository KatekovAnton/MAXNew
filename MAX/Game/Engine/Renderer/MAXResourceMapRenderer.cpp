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

int MAXResourceMapRenderer::GetTileIndexForResourceTypeAndAmount(RESOURCE_TYPE type, int amount) const
{
    return 0;
}

MAXResourceMapRenderer::MAXResourceMapRenderer(int mapW, int mapH)
:_mesh(NULL), _texture(NULL), _cells(new bool[mapH * mapW]), _mapW(mapW), _mapH(mapH)
{
    memset(_cells, 0, mapH * mapW);
    
    
    
    
    
}

MAXResourceMapRenderer::~MAXResourceMapRenderer()
{
    delete _mesh;
    delete _texture;
    delete[] _cells;
    delete [] _tileInformation;
}

void MAXResourceMapRenderer::AddCellToScan(int x, int y)
{
    
}