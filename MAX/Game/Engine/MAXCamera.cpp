//
//  MAXCamera.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXCamera.h"
#include "Display.h"

MAXCamera::MAXCamera(GRect2D bounds)
{
    
    _aspectRatio = fabsf(bounds.size.width / bounds.size.height);
    _far = 100.0f;
//    projection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f),
//                                           _aspectRatio,
//                                           0.1f,
//                                           _far);
    position = GLKVector3Make(0, 0, 0);
    _displayScale = Display::currentDisplay()->GetDisplayScale();
    _scalex = _displayScale*64.0/bounds.size.width;
    _scaley = _displayScale*64.0/bounds.size.height;
    
    scale = 1.0;
    
    this->view = GLKMatrix4Identity;
    
    projection = GLKMatrix4MakeScale(_scalex, _scaley, 1);
    minDepth = 0.1;
    maxDepth = 100.0;
    screenSize = bounds.size;
    
    changed = true;
}

void MAXCamera::Scale(float deltaScale)
{
    scale/=deltaScale;
    if (scale<0.5)
        scale = 0.5;
    if (scale>20) 
        scale = 20;
    changed = true;
}

void MAXCamera::Move(float deltax, float deltay)
{
    position.x += deltax * _scalex * scale/( _displayScale * 2.0);
    position.y -= deltay * _scalex * scale/( _displayScale * 2.0);
    changed = true;
}

void MAXCamera::Update()
{
    if (!changed)
        return;
    changed = false;
    RecalculateViewMatrix();
}

void MAXCamera::RecalculateViewMatrix()
{
    this->view = GLKMatrix4Multiply(GLKMatrix4MakeScale(1.0/scale, 1.0/scale, 1), GLKMatrix4MakeTranslationV(position));
}