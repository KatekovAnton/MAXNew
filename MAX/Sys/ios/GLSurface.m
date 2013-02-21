//
//  GLSurface.m
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/18/12.
//
//

#import "GLSurface.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#include "Sys.h"
@implementation GLSurface

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame andDepthFormat:(unsigned int)depthFormat andPixelFormat:(unsigned int)pixelFormat {
    self = [super initWithFrame:frame];
    if (self) {
        SysLogInfo("Initializing iOS GL Surface");
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        //we don't want a transparent surface
        eaglLayer.opaque = TRUE;
        NSString *_iosPixelFormat = kEAGLColorFormatRGB565;
        if (pixelFormat != GL_RGB565) {
            _iosPixelFormat = kEAGLColorFormatRGBA8;
        }
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil];
        
        
        _glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        NSAssert(_glContext, @" OpenGL ES 2.0 API Support required ");
        
        [EAGLContext setCurrentContext:_glContext];
        
        _depthFormat = depthFormat;
        _pixelFormat = pixelFormat;
        
        glGenFramebuffers(1, &_framebuffer);
        
        glGenRenderbuffers(1, &_renderbuffer);
        
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffer);
    }
    return self;
}

- (void)layoutSubviews {
    [self setContentScaleFactor:[UIScreen mainScreen].scale];
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    
    [_glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];
    
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);

    if (_depthFormat)
	{
		if( ! _depthbuffer ) {
			glGenRenderbuffers(1, &_depthbuffer);
			NSAssert(_depthbuffer, @"Can't create depth buffer");
		}
        
		glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
		
        glRenderbufferStorage(GL_RENDERBUFFER, _depthFormat, _backingWidth, _backingHeight);
        
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);
        
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
	}
    
    
}

- (void)dealloc {
    
    [EAGLContext setCurrentContext:nil];
    [_glContext release];
    _glContext = nil;
    
    [super dealloc];
}


@end
