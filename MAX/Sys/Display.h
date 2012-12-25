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
#include "platform/CCEGLViewProtocol.h"
#include "DisplayPinchDelegate.h"
#include <iostream>
#include GLES2

using namespace cocos2d;

class Display : public CCEGLViewProtocol {
    static Display * _current;
    
public:
    static Display * currentDisplay();
    
    virtual void  InitGLSurface(unsigned int depthFormat, unsigned int pixelFormat);
    
    virtual GLfloat GetDisplayHeight();
    virtual GLfloat GetDisplayWidth();
    
    virtual GLfloat GetDisplayScale();
    
    virtual void SwapBuffers();
    
    void SetViewPortInPoints(float x , float y , float w , float h);
    void SetScissorInPoints(float x , float y , float w , float h);

    virtual void SetPinchDelegate(DisplayPinchDelegate* delegate);
};

#endif /* defined(__TerminalExtraction__Display__) */
