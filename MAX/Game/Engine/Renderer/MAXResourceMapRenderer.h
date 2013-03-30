//
//  MAXResourceMapRenderer.h
//  MAX
//
//  Created by  Developer on 26.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXResourceMapRenderer__
#define __MAX__MAXResourceMapRenderer__

#include <iostream>
#include "miniPrefix.h"

class EngineTiledDynamicMesh;
class Texture;
class MAXREsourceMapMaterial;
class Shader;

struct TileInformation;

class MAXResourceMapRenderer {
    
    EngineTiledDynamicMesh  *_mesh;
    TileInformation         *_tileInformation;
    MAXREsourceMapMaterial  *_material;
    bool                    *_cells;
    
    int _mapW;
    int _mapH;
    
public:
    
    int GetIndexForCoordinates(const int x, const int y) const;
    int GetTileIndexForResourceTypeAndAmount(const RESOURCE_TYPE type, const unsigned char amount) const;
    
    MAXResourceMapRenderer(const int mapW, const int mapH);
    ~MAXResourceMapRenderer();
    
    void AddCellToScan(const int x, const int y, const RESOURCE_TYPE type, const unsigned char amount);
    
    void Draw(Shader *shader);

};

#endif /* defined(__MAX__MAXResourceMapRenderer__) */
