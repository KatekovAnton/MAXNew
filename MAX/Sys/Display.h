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
    
    virtual void SwapBuffers();
    
    void setViewPortInPoints(float x , float y , float w , float h);
    void setScissorInPoints(float x , float y , float w , float h);

};

#endif /* defined(__TerminalExtraction__Display__) */
