//
//  MAXUnitMaterial.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXUnitMaterial.h"
#include "Shader.h"
#include "Texture.h"

const double MAXUnitMaterialframeTime = 1.0;

MAXUnitMaterial::MAXUnitMaterial(int _frameCount)
{
    frameCount = _frameCount;
    frames = new MAXUnitMaterialFrame[frameCount];
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
//    time = time + elapsedTime;
//    
//    int f = (time/MAXUnitMaterialframeTime);
//    index = f%frameCount;    
}

void MAXUnitMaterial::ApplyLod(int lod, Shader *shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[index]->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
}

void MAXUnitMaterial::SetFrame(int frame)
{}

void MAXUnitMaterial::StopBackgroundLoading()
{}