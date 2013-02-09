//
//  MAXCamera.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXCamera__
#define __MAX__MAXCamera__

#include <iostream>
#include "Geometry.h"

class MAXCamera {
    void RecalculateViewMatrix();
    //how many cells can be displayed on scrren by width
    GLfloat _scalex;
    //how many cells can be displayed on scrren by height
    GLfloat _scaley;
    //devise display scale, retina = 2
    GLfloat _displayScale;
    
public:
    
    bool changed;
    //100
    GLfloat             maxDepth;
    //0.1
    GLfloat             minDepth;
    GLfloat             _far;
    GLfloat             _aspectRatio;
    GSize2D             screenSize;
    
    GLKMatrix4          projection;
    GLKMatrix4          view;
    
    //current game camera scale
    GLfloat             scale;
    //current game camera position
    GLKVector3          position;
    
    MAXCamera(GRect2D bounds);
    
    //scale camera, on touches
    void Scale(float deltaScale);
    //move camera
    void Move(float deltax, float deltay);
    //update camra, recalculates values
    void Update();
    
};

#endif /* defined(__MAX__MAXCamera__) */
