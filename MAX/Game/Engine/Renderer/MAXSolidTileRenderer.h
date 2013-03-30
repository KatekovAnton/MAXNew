//
//  MAXSolidTileRenderer.h
//  MAX
//
//  Created by  Developer on 30.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXSolidTileRenderer__
#define __MAX__MAXSolidTileRenderer__

#include <iostream>
#include "miniPrefix.h"
#include "EngineTiledDynamicMeshDelegate.h"

class EngineTiledDynamicMesh;
class Texture;
class MAXREsourceMapMaterial;
class Shader;

class MAXSolidTileRenderer : public EngineTiledDynamicMeshDelegate {
    
    EngineTiledDynamicMesh  *_mesh;
    int                     *_cells;
    
    int _mapW;
    int _mapH;
    
public:
    
    GLKVector4 color;
    
    int GetIndexForCoordinates(const int x, const int y) const;
    
    MAXSolidTileRenderer(const int mapW, const int mapH);
    ~MAXSolidTileRenderer();
    
    void CompletlyFillMap();
    void AddCell(const int x, const int y);
    void RemoveCell(const int x, const int y);
    
    void Draw(Shader *shader);
    
#pragma mark - EngineTiledDynamicMeshDelegate
    virtual void ElementDidChangePosition(int oldIndex, int newIndex, int singleArrayIndex);
    
};

#endif /* defined(__MAX__MAXFogRenderer__) */
