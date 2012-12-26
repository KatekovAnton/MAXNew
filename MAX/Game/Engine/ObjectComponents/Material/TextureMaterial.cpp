//
//  TextureMaterial.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "TextureMaterial.h"
#include "Shader.h"
#include "Texture.h"

TextureMaterial::TextureMaterial():Material()
{
    _texture = NULL;
}

TextureMaterial::TextureMaterial(string name):Material()
{
    _texture = new Texture(name);
}

void TextureMaterial::ApplyLod(int lod, Shader *shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
}

void TextureMaterial::SetTexture(Texture* texture)
{
    if (_texture)
        delete _texture;
    
    _texture = texture;
}

TextureMaterial::~TextureMaterial()
{
    delete _texture;
}

void TextureMaterial::SetFrame(int frame)
{
    
}

void TextureMaterial::StopBackgroundLoading()
{
    
}