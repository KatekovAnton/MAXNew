//
//  MAXUnitMaterial.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXUnitMaterial.h"
#include "Texture.h"

MAXUnitMaterial::MAXUnitMaterial(int _frameCount)
{
    frameCount = _frameCount;
    frames = new FrameInfo[frameCount];
    textures = new Texture*[frameCount];
    
    
    index = 0;
    time = 0;
}

MAXUnitMaterial::~MAXUnitMaterial()
{
    delete [] frames;
    for (int i = 0; i < frameCount; i++) 
        delete textures[i];
    delete [] textures;
}

void MAXUnitMaterial::DoFrame(double elapsedTime)
{
    time = time + elapsedTime;
}

void MAXUnitMaterial::ApplyLod(int lod, Shader *shader)
{}

void MAXUnitMaterial::SetFrame(int frame)
{}

void MAXUnitMaterial::StopBackgroundLoading()
{}