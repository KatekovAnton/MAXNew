//
//  Texture.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/3/12.
//
//

#ifndef __TerminalExtraction__Texture__
#define __TerminalExtraction__Texture__

#include <iostream>
#include "Geometry.h"

using namespace std;

class Texture {
    
    GLuint _filter;
    
    GLuint          _name;
    bool            _fromFile;
    
    GLfloat         _texWidth;
    GLfloat         _texHeight;
    
    string          _textureFilename;
    
    GLuint SetupTexture(GLubyte *spriteData);
    
    bool _disposed;
    
public:
    
    Texture();
    Texture(GLuint filter, GLubyte *spriteData, int texWidth, int texHeight);
    Texture(string filename);
    
    GLuint GetTextureName();
    
    
    virtual void Dispose();
    virtual void LoadFromBuffer(char *buffer, unsigned int bufferLength);
    
    ~Texture();
};

#endif /* defined(__TerminalExtraction__Texture__) */
