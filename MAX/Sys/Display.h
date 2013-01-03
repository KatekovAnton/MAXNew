//
//  Display.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#ifndef __TerminalExtraction__Display__
#define __TerminalExtraction__Display__
#include "GlobalConstants.h"
#ifdef TARGET_OS_IPHONE
#include GLES2
#endif

#ifdef TARGET_OS_WIN
#include <windows.h>
#include "glew.h"
#include "glext.h"
#include "wchar.h"
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "DisplayPinchDelegate.h"
#include <iostream>
#include "platform/CCEGLViewProtocol.h"

using namespace cocos2d;

class Display : public CCEGLViewProtocol {
    static Display * _current;
    
public:
    static Display * currentDisplay();
    
    virtual void  InitGLSurface(unsigned int depthFormat, unsigned int pixelFormat);
    
    virtual GLfloat GetDisplayHeight();
    virtual GLfloat GetDisplayWidth();
    
    virtual GLfloat GetDisplayScale();
    
    virtual void SwapOGLBuffers();
    
    void SetViewPortInPoints(float x , float y , float w , float h);
    void SetScissorInPoints(float x , float y , float w , float h);

    virtual void SetPinchDelegate(DisplayPinchDelegate* delegate);
};

#endif /* defined(__TerminalExtraction__Display__) */
