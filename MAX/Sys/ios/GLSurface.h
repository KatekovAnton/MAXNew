//
//  GLSurface.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGLDrawable.h>

@interface GLSurface : UIView {
@private
    EAGLContext     *_glContext;
    
    unsigned int        _depthFormat;
    unsigned int        _pixelFormat;
    bool                _multiSampling;
    
    GLuint              _framebuffer;
    GLuint              _depthbuffer;
    GLuint              _renderbuffer;
    
    GLint             _backingWidth;
    GLint             _backingHeight;
}

- (id)initWithFrame:(CGRect)frame andDepthFormat:(unsigned int)depthFormat andPixelFormat:(unsigned int)pixelFormat;

@end
