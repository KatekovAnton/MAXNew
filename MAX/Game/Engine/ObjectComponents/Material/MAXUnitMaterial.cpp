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

void MAXUnitMaterial::SetImagesCount(int frameCount, int shadowframeCount)
{
    _frameCount = frameCount;
    _shadowframeCount = shadowframeCount;
    frames = new MAXUnitMaterialFrame[frameCount];
    textures = new Texture*[frameCount];
    if (shadowframeCount != 0)
    {
        shadowframes = new MAXUnitMaterialFrame[shadowframeCount];
        shadowTextures = new Texture*[shadowframeCount];
    }
}

MAXUnitMaterial::~MAXUnitMaterial()
{
    delete [] frames;
    for (int i = 0; i < _frameCount; i++)
        delete textures[i];
    delete [] textures;
    if (_shadowframeCount != 0) {
        delete [] shadowframes;
        for (int i = 0; i < _shadowframeCount; i++)
            delete shadowTextures[i];
        delete [] shadowTextures;
    }
}

void MAXUnitMaterial::DoFrame(double elapsedTime)
{}

void MAXUnitMaterial::ApplyPalette(Shader *shader, Texture* _palette)
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _palette->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE1], 1);
}

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
    
}

void MAXUnitMaterial::SetFrame(int frame)
{}

void MAXUnitMaterial::StopBackgroundLoading()
{}


