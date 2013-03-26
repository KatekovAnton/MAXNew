//
//  MAXGrid.h
//  MAX
//
//  Created by  Developer on 29.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXGrid__
#define __MAX__MAXGrid__

#include <iostream>
#include "Geometry.h"

class MAXGrid {
    
    int _mapw;
    int _maph;
    
    GLKVector2* _startsW;
    GLKVector2* _endsW;
    GLKVector2* _startsH;
    GLKVector2* _endsH;
    
    void freeArrays();
    
public:
    
    float cameraScale;
    
    float startx;
    int countx;
    float starty;
    int county;
    
    float step;
    
    MAXGrid();
    ~MAXGrid();
    
    
    void SetMapSize(float mapw, float maph);
    void UpdateInfo(bool needNewSize);
    void DrawGrid();
};

#endif /* defined(__MAX__MAXGrid__) */
