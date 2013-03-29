//
//  MAXREsourceMapMaterial.cpp
//  MAX
//
//  Created by Anton Katekov on 29.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXREsourceMapMaterial.h"
#include "Shader.h"
#include "Texture.h"
#include "MAXEngine.h"
#include "MAXContentLoader.h"

MAXREsourceMapMaterial::MAXREsourceMapMaterial()
{
    _texture_w = MAXSCL->resourceTiles;
}

MAXREsourceMapMaterial::~MAXREsourceMapMaterial()
{}

void MAXREsourceMapMaterial::ApplyLod(int lod, Shader *shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_w->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
}

void MAXREsourceMapMaterial::SetFrame(int frame)
{}

void MAXREsourceMapMaterial::StopBackgroundLoading()
{}