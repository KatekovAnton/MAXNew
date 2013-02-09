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

MAXUnitMaterial::MAXUnitMaterial()
:index(0), time(0)
{
    
}

void MAXUnitMaterial::SetImagesCount(int _frameCount)
{
    frameCount = _frameCount;
    frames = new MAXUnitMaterialFrame[frameCount];
    textures = new Texture*[frameCount];
    
    shadowframes = new MAXUnitMaterialFrame[8];
    shadowTextures = new Texture*[8];
}

MAXUnitMaterial::~MAXUnitMaterial()
{
    delete [] frames;
    delete [] shadowframes;
    for (int i = 0; i < frameCount; i++) 
        delete textures[i];
    for (int i = 0; i < 0; i++)
        delete shadowTextures[i];
    delete [] textures;
    delete [] shadowTextures;
    delete pallete;
}

void MAXUnitMaterial::DoFrame(double elapsedTime)
{}

void MAXUnitMaterial::ApplyShadowLod(int lod, Shader *shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowTextures[index]->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
}

void MAXUnitMaterial::ApplyLod(int lod, Shader *shader)
{
//    if (index<8) {
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, shadowTextures[index]->GetTextureName());
//        glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
//    }
//    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[index]->GetTextureName());
        glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
    }
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pallete->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE1], 1);
}

void MAXUnitMaterial::SetFrame(int frame)
{}

void MAXUnitMaterial::StopBackgroundLoading()
{}


