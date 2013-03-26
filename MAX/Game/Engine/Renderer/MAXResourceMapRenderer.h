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

class MAXResourceMapRenderer {
    
    EngineTiledDynamicMesh  *_mesh;
    Texture                 *_texture;
    
    bool                    *_cells;
    
    int _mapW;
    int _mapH;
    
public:
    
    
    int TileIndexForResourceTypeAndAmount(RESOURCE_TYPE type, int amount) const;
    
    MAXResourceMapRenderer(int mapW, int mapH);
    ~MAXResourceMapRenderer();
    
    void AddCellToScan(int x, int y);

};

#endif /* defined(__MAX__MAXResourceMapRenderer__) */
