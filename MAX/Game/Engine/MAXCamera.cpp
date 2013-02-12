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

# define MIN(x,y) ((x)<(y)?(x):(y))

#define DEFAULT_SCALE 0.5

#define DEFAULT_MAP_PART 112

#define DEFAULT_CELL_SIZE 64.0

MAXCamera::MAXCamera(GRect2D bounds)
{
    
    _aspectRatio = fabsf(bounds.size.width / bounds.size.height);
    _far = 100.0f;
    position = GLKVector3Make(0, 0, 0);
    _displayScale = Display::currentDisplay()->GetDisplayScale();
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

void MAXCamera::Scale(float deltaScale)
{
    scale/=deltaScale;
       
    if(scale>maxScale)
        scale = maxScale;
    Move(0, 0);
    changed = true;
}

void MAXCamera::Move(float deltax, float deltay)
{
    position.x += deltax * _scalex * scale/( _displayScale * 2.0);
    position.y -= deltay * _scalex * scale/( _displayScale * 2.0);
    
    CCRect rect = engine->ScreenToWorldRect();
    if (rect.origin.x<0)
    {
        //move camera to right
        position.x += rect.origin.x/64.0;
    }
    else if (rect.size.width + rect.origin.x > mapW * DEFAULT_CELL_SIZE)
    {
        //move camera to left
        position.x += (rect.size.width + rect.origin.x - mapW * DEFAULT_CELL_SIZE)/64.0;
    }
    
    if (rect.origin.y<0)
    {
        //move camera bottom
        position.y -= rect.origin.y/64.0;
    }
    else if (rect.size.height + rect.origin.y > mapH * DEFAULT_CELL_SIZE)
    {
        //move camera top
        position.y -= (rect.size.height + rect.origin.y - mapH * DEFAULT_CELL_SIZE)/64.0;
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
//    GLfloat screenCurrentWidth = scale*screenSize.width/DEFAULT_CELL_SIZE;//Screen width in cells
//    GLfloat screenCurrentHeight = scale*screenSize.height/DEFAULT_CELL_SIZE;//Screen width in cells
//    
//    if(position.x+screenCurrentWidth/2>mapW/2){
//        position.x = (mapW-screenCurrentWidth)/2;
//    }else if(position.x-screenCurrentWidth/2<-mapW/2){
//        position.x = (-mapW+screenCurrentWidth)/2;
//    }
//    
//    if(position.y+screenCurrentHeight/2>mapH/2){
//        position.y = (mapH-screenCurrentHeight)/2;
//    }else if(position.y-screenCurrentHeight/2<-mapH/2){
//        position.y = (-mapH+screenCurrentHeight)/2;
//    }
    
    this->view = GLKMatrix4Multiply(GLKMatrix4MakeScale(1.0/scale, 1.0/scale, 1), GLKMatrix4MakeTranslationV(position));
}