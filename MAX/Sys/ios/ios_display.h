//
//  ios_display.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#ifndef __TerminalExtraction__ios_display__
#define __TerminalExtraction__ios_display__

#include <iostream>
#include "Display.h"

class iosDisplay : public Display {
    
public:
    
    iosDisplay();
    
    virtual void InitGLSurface(unsigned int depthFormat, unsigned int pixelFormat);
    
    virtual GLfloat GetDisplayHeight();
    virtual GLfloat GetDisplayWidth();
    
    virtual void    SwapBuffers();
    
    virtual bool    isOpenGLReady();
    virtual bool    setContentScaleFactor(float contentScaleFactor);
    
    // keep compatible
    virtual void    end();
    virtual void    swapBuffers();
    
    virtual void setIMEKeyboardState(bool bOpen);
};

#endif /* defined(__TerminalExtraction__ios_display__) */
