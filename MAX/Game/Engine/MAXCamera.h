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
    
    GLfloat _scalex;
    GLfloat _scaley;
    GLfloat _displayScale;
    
public:
    
    GLfloat             maxDepth;
    GLfloat             minDepth;
    GLfloat             _far;
    GLfloat             _aspectRatio;
    GSize2D             screenSize;
    
    GLKMatrix4          projection;
    GLKMatrix4          view;
    
    GLfloat             scale;
    GLKVector3          position;
    
    MAXCamera(GRect2D bounds);
    
    void Scale(float deltaScale);
    void Move(float deltax, float deltay);
    
    void Update();
    
};

#endif /* defined(__MAX__MAXCamera__) */
