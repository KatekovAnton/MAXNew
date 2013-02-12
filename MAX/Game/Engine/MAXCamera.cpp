//
//  MAXCamera.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXCamera.h"
#include "Display.h"

#define MINSCALE 0.25
#define MAXSCALE 3.5

#define DEFAULT_SCALE 0.5

#define DEFAULT_MAP_PART 112

MAXCamera::MAXCamera(GRect2D bounds)
{
    
    _aspectRatio = fabsf(bounds.size.width / bounds.size.height);
    _far = 100.0f;
    position = GLKVector3Make(0, 0, 0);
    _displayScale = Display::currentDisplay()->GetDisplayScale();
    _scalex = _displayScale*64.0/bounds.size.width;
    _scaley = _displayScale*64.0/bounds.size.height;
    
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
}

void MAXCamera::Scale(float deltaScale)
{
    scale/=deltaScale;
    if (scale<MINSCALE)
        scale = MINSCALE;
    if (scale>MAXSCALE) 
        scale = MAXSCALE;
    changed = true;
}

void MAXCamera::Move(float deltax, float deltay)
{
    position.x += deltax * _scalex * scale/( _displayScale * 2.0);
    position.y -= deltay * _scaley * scale/( _displayScale * 2.0);
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
    GLfloat screenCurrentWidth = scale*screenSize.width*(_displayScale * 2.0)/64.0;//Screen width in cells
    GLfloat screenCurrentHeight = scale*screenSize.height*(_displayScale * 2.0)/64.0;//Screen width in cells
    
    if(position.x+screenCurrentWidth/2>mapW/2){
        position.x = (mapW-screenCurrentWidth)/2;
    }else if(position.x-screenCurrentWidth/2<-mapW/2){
        position.x = (-mapW+screenCurrentWidth)/2;
    }
    
    if(position.y+screenCurrentHeight/2>mapH/2){
        position.y = (mapH-screenCurrentHeight)/2;
    }else if(position.y-screenCurrentHeight/2<-mapH/2){
        position.y = (-mapH+screenCurrentHeight)/2;
    }
    
    this->view = GLKMatrix4Multiply(GLKMatrix4MakeScale(1.0/scale, 1.0/scale, 1), GLKMatrix4MakeTranslationV(position));
}