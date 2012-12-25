//
//  Texture.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "Texture.h"
#include "FileManger.h"

Texture::Texture()
{
    
}

Texture::Texture(string filename)
{
    
    FileManager *fm = FileManager::CreateManager();
    GLubyte *textureData = fm->CreateBitmapData(filename, &_texWidth, &_texHeight);
    
    _name = SetupTexture(textureData);
    _textureFilename = filename;
    _fromFile = true;
}


GLuint Texture::SetupTexture(GLubyte *spriteData)
{
    GLuint texName;
    
    glGenTextures(1, &texName);
    
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texWidth, _texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    free(spriteData);
    
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf("Error uploading texture. glError: 0x%04X", err);
    
    _disposed = false;
    return texName;
}

GLuint Texture::GetTextureName()
{
    return _name;
}

void Texture::Dispose()
{
    if(_disposed)
        return;
    
    
   glDeleteTextures(1, &_name);
    _disposed = true;
}

void Texture::LoadFromBuffer(char *buffer, unsigned int bufferLength)
{
    FileManager *fm = FileManager::CreateManager();
    GLubyte *textureData = fm->CreateBitmapData(buffer, bufferLength, &_texWidth, &_texHeight);
    
    _name = SetupTexture(textureData);
    _disposed = false;
}

Texture::~Texture()
{
    Dispose();
}