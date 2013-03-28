//
//  EngineTiledDynamicMesh.h
//  MAX
//
//  Created by  Developer on 26.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__EngineTiledDynamicMesh__
#define __MAX__EngineTiledDynamicMesh__

#include <iostream>
#include "GlobalConstants.h"
#include "USimpleContainer.h"

struct Polygon {
    vertexStruct vertexTL;
    vertexStruct vertexBR;
    vertexStruct vertexTR;
    vertexStruct vertexBR1;
    vertexStruct vertexBL;
    vertexStruct vertexTL1;
};

using namespace Utils;

struct EngineTiledDynamicMeshTextureInfo {
    int tileCountW;
    int tileCountH;
};

class EngineTiledDynamicMesh {
    
public:
    
    USimpleContainer<Polygon> *_vertices;
    USimpleContainer<int> *_indices;
    
    EngineTiledDynamicMeshTextureInfo _textureInfo;
    
    EngineTiledDynamicMesh(EngineTiledDynamicMeshTextureInfo textureInfo);
    ~EngineTiledDynamicMesh();
    
    void AddPolygon(int x, int y, int tileIndex);
    
};

#endif /* defined(__MAX__EngineTiledDynamicMesh__) */
