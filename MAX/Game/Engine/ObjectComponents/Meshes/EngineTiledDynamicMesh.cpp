//
//  EngineTiledDynamicMesh.cpp
//  MAX
//
//  Created by  Developer on 26.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "EngineTiledDynamicMesh.h"

EngineTiledDynamicMesh::EngineTiledDynamicMesh(EngineTiledDynamicMeshTextureInfo textureInfo)
:_textureInfo(textureInfo), _vertices(new USimpleContainer<Polygon>(100)), _indices(new USimpleContainer<int>(100))
{
    
}

EngineTiledDynamicMesh::~EngineTiledDynamicMesh()
{}

void EngineTiledDynamicMesh::AddPolygon(int x, int y, int tileIndex)
{}