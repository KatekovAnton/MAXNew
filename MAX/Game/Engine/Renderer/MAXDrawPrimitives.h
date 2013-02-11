//
//  MAXDrawPrimitives.h
//  MAX
//
//  Created by Anton Katekov on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXDrawPrimitives__
#define __MAX__MAXDrawPrimitives__

#include <iostream>
#include "miniPrefix.h"

class Shader;

class MAXDrawPrimitives {
    
    Shader* _shader;
    GLKMatrix4 _projectionMatrix;
    
    GLKVector4 vertices[2];
    
public:
    
    GLKVector4 _color;
    
    
    
    MAXDrawPrimitives();
    ~MAXDrawPrimitives();
    
    static MAXDrawPrimitives* SharedDrawPrimitives();
    
    void Begin();
    void BindColor();
    void DrawLine(CCPoint from, CCPoint to);
    
    void DrawCircle( const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY);
    void DrawCircle( const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter);
};

#endif /* defined(__MAX__MAXDrawPrimitives__) */
