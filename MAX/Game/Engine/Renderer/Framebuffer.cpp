//
//  Framebuffer.cpp
//  MAX
//
//  Created by Anton Katekov on 15.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "Framebuffer.h"
#ifdef TARGET_OS_WIN
#include <windows.h>
#include "glew.h"
#include "glext.h"
#include "wchar.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define glDeleteFramebuffers glDeleteFramebuffersEXT
#define glGenFramebuffers glGenFramebuffersEXT
#define glBindFramebuffer glBindFramebufferEXT
#define glFramebufferTexture2D glFramebufferTexture2DEXT
#define	glCheckFramebufferStatus glCheckFramebufferStatusEXT

#define GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#define GL_FRAMEBUFFER_BINDING GL_FRAMEBUFFER_BINDING_EXT
#define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT

#endif

Framebuffer::Framebuffer(GLKVector2 sz)
{
    this->initFB(sz);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &_colorTexture);
    //glDeleteTextures(1, &_depthTexture);
    glDeleteFramebuffers(1, &_fbo);
}

void Framebuffer::initFB(GLKVector2 sz)
{
    _w = sz.x;
    _h = sz.y;
    // create a frame buffer object
    GLuint _local;
    glGenFramebuffers(1, &_local);
    _fbo = _local;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_lastfbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    
    // set up color texture
    // generate a texture id
    glGenTextures(1, &_colorTexture);
    // bind the texture
    glBindTexture(GL_TEXTURE_2D, _colorTexture);
    // set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    // create the texture in the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 sz.x, sz.y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    

    // attach the color texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTexture, 0);
    // attach depth buffer
    // glFramebufferTexture2D(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_TEXTURE_2D, _depthTexture, 0);
    
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(status != GL_FRAMEBUFFER_COMPLETE)
        printf("Error making complete framebuffer object 0x%04X", status);
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, _lastfbo);
        
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            printf("Error creating framebuffer. glError: 0x%04X", err);
            
            
}

void Framebuffer::bind()
{
    glGetFloatv(GL_VIEWPORT, (GLfloat*)&_viewport);
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_lastfbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _w, _h);
    glGetFloatv(GL_COLOR_CLEAR_VALUE,oldClearColor_);
    
	// BUG XXX: doesn't work with RGB565.
	glClearColor(1,1,1,1);
    
	// BUG #631: To fix #631, uncomment the lines with #631
	// Warning: But it CCGrabber won't work with 2 effects at the same time
    //	glClearColor(0.0f,0.0f,0.0f,1.0f);	// #631
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _lastfbo);
    glClearColor( oldClearColor_[0], oldClearColor_[1], oldClearColor_[2], oldClearColor_[3] );
    glViewport(_viewport.x, _viewport.y, _viewport.w, _viewport.h);
}
