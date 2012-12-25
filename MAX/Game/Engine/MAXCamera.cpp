//
//  MAXCamera.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXCamera.h"

MAXCamera::MAXCamera(GRect2D bounds)
{
    this->view = GLKMatrix4Identity;
    
    _aspectRatio = fabsf(bounds.size.width / bounds.size.height);
    _far = 100.0f;
    projection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f),
                                           _aspectRatio,
                                           0.1f,
                                           _far);
    
    minDepth = 0.1;
    maxDepth = 100.0;
    screenSize = bounds.size;
}