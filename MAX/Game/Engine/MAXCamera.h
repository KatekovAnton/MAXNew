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
public:
    
    GLfloat             maxDepth;
    GLfloat             minDepth;
    GLfloat             _far;
    GLfloat             _aspectRatio;
    GSize2D             screenSize;
    
    GLKMatrix4          projection;
    GLKMatrix4          view;
    
    MAXCamera(GRect2D bounds);
};

#endif /* defined(__MAX__MAXCamera__) */
