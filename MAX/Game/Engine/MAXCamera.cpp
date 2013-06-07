//
//  MAXCamera.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXCamera.h"
#include "Display.h"
#include "MAXEngine.h"

#define DEFAULT_SCALE 1
#define MIN_SCALE 0.25
#define DEFAULT_MAP_PART 112
#define DEFAULT_CELL_SIZE 64.0

MAXCamera::MAXCamera(GRect2D bounds, float displayScale)
:minZoom(false)
{
    _bounds = bounds;
    _aspectRatio = fabsf(bounds.size.width / bounds.size.height);
    _far = 100.0f;
    position = GLKVector3Make(0, 0, 0);
    _displayScale = displayScale;//Display::currentDisplay()->GetDisplayScale();
    _scalex = _displayScale*DEFAULT_CELL_SIZE/bounds.size.width;
    _scaley = _displayScale*DEFAULT_CELL_SIZE/bounds.size.height;
    
    scale = DEFAULT_SCALE;
    
    mapH = DEFAULT_MAP_PART;
    mapW = DEFAULT_MAP_PART;
    
    this->view = GLKMatrix4Identity;
    
    projection = GLKMatrix4MakeScale(_scalex, _scaley, 1);
    minDepth = 0.1;
    maxDepth = 100.0;
    screenSize = bounds.size;
    
    changed = true;
}

void MAXCamera::SetMapSize(int w, int h)
{
    mapH = h;
    mapW = w;
    changed = true;
    
    float maxscalew = w * DEFAULT_CELL_SIZE/screenSize.width * _displayScale / 2.0;
    float maxscaleh = h * DEFAULT_CELL_SIZE/screenSize.height * _displayScale / 2.0;
    float resultscale = MIN(maxscaleh, maxscalew);
    
    maxScale = resultscale;
}

void MAXCamera::SetPosition(const GLKVector3 &position1)
{
    position = position1;
    Move(0, 0);
    changed = true;
}

void MAXCamera::Scale(float deltaScale)
{
    minZoom = false;
    scale/=deltaScale;
    if (scale<MIN_SCALE) 
        scale = MIN_SCALE;
    
    if(scale>maxScale)
    {
        scale = maxScale;
        minZoom = true;
    }
    Move(0, 0);
    changed = true;
}

void MAXCamera::Move(float deltax, float deltay)
{
    CCRect rect = engine->ScreenToWorldRect();
    float screenMX = rect.size.width / 64.0;
    float screenMY = rect.size.height / 64.0;
    
    CCSize sz = CCSize(_bounds.size.width / _displayScale, _bounds.size.height / _displayScale);
    float rdx = screenMX * deltax / sz.width;
    float rdy = screenMY * deltay / sz.height;
    
    position.x += rdx;
    position.y -= rdy;
    
    
    position.x = floorf(position.x * 100) / 100;
    position.y = floorf(position.y * 100) / 100;
//    {
//        int c = position.x * 64.0;
//        position.x = c / 64.0 + 0.5/128.0;
//    }
//    {
//        int c = position.y * 64.0;
//        position.y = c / 64.0 + 0.5/128.0;
//    }
    
    rect = engine->ScreenToWorldRect();
    if (rect.origin.x<0)
    {
        //move camera to right
        position.x += rect.origin.x/DEFAULT_CELL_SIZE;
    }
    else if (rect.size.width + rect.origin.x > mapW * DEFAULT_CELL_SIZE)
    {
        //move camera to left
        position.x += (rect.size.width + rect.origin.x - mapW * DEFAULT_CELL_SIZE)/DEFAULT_CELL_SIZE;
    }
    
    if (rect.origin.y<0)
    {
        //move camera bottom
        position.y -= rect.origin.y/DEFAULT_CELL_SIZE;
    }
    else if (rect.size.height + rect.origin.y > mapH * DEFAULT_CELL_SIZE)
    {
        //move camera top
        position.y -= (rect.size.height + rect.origin.y - mapH * DEFAULT_CELL_SIZE)/DEFAULT_CELL_SIZE;
    }
    
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


