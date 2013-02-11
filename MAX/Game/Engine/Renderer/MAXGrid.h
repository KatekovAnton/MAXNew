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
#include <cocos2d.h>

using namespace cocos2d;

class MAXGrid {
    
    int _mapw;
    int _maph;
    
    CCPoint* _startsW;
    CCPoint* _endsW;
    CCPoint* _startsH;
    CCPoint* _endsH;
    
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
