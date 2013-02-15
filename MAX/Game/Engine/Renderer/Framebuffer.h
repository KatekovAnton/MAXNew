//
//  Framebuffer.h
//  MAX
//
//  Created by Anton Katekov on 15.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Framebuffer__
#define __MAX__Framebuffer__

#include <iostream>
#include "miniPrefix.h"


class Framebuffer {
    GLuint _fbo;
    GLuint _colorTexture; 
    GLint _lastfbo;
    GLfloat	oldClearColor_[4];
    
    void initFB(GLKVector2 sz);

public:
    GLuint GetColorTexture() const { return _colorTexture; };

    Framebuffer(GLKVector2 size);
    ~Framebuffer();
    
    void bind();
    void unbind();
};

#endif /* defined(__MAX__Framebuffer__) */
