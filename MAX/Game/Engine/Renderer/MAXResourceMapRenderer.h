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

struct TileInformation;

class MAXResourceMapRenderer {
    
    EngineTiledDynamicMesh  *_mesh;
    Texture                 *_texture;
    TileInformation          *_tileInformation;
    bool                    *_cells;
    
    int _mapW;
    int _mapH;
    
public:
    
    int GetIndexForCoordinates(int x, int y) const;
    int GetTileIndexForResourceTypeAndAmount(RESOURCE_TYPE type, unsigned char amount) const;
    
    MAXResourceMapRenderer(int mapW, int mapH);
    ~MAXResourceMapRenderer();
    
    void AddCellToScan(int x, int y, RESOURCE_TYPE type, unsigned char amount);
    
    void Draw();

};

#endif /* defined(__MAX__MAXResourceMapRenderer__) */
