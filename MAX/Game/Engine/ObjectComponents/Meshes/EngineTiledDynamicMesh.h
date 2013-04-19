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
#include <map>
#include "GlobalConstants.h"
#include "USimpleContainer.h"

struct MAXPolygon {
    vertexStruct vertexTL;
    vertexStruct vertexBR;
    vertexStruct vertexTR;
    vertexStruct vertexBR1;
    vertexStruct vertexBL;
    vertexStruct vertexTL1;
};

using namespace Utils;
using namespace std;

struct EngineTiledDynamicMeshTextureInfo {
    int tileCountW;
    int tileCountH;
};

class EngineTiledDynamicMeshDelegate;

class EngineTiledDynamicMesh {
    int _mapW;
    int _mapH;
public:
    
    int *meshIndexToCoordianteHash;
    
    EngineTiledDynamicMeshDelegate *_delegate_w;
    
    USimpleContainer<MAXPolygon> *_vertices;
    
    EngineTiledDynamicMeshTextureInfo _textureInfo;
    
    EngineTiledDynamicMesh(EngineTiledDynamicMeshTextureInfo textureInfo, int mapW, int mapH);
    ~EngineTiledDynamicMesh();
    
    int AddPolygon(int x, int y, int tileIndex, int singleArrayIndex);
    void RemovePolygon(int index);
    void Clear();
    void Draw();
};

#endif /* defined(__MAX__EngineTiledDynamicMesh__) */
