//
//  Display.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#include "Display.h"
#include "ios_display.h"

Display * Display::currentDisplay() {
//    if (!_current) {
//#ifdef TARGET_OS_IPHONE
//        _current = new iosDisplay();
//#else
//        
//#endif
//    }
    return new iosDisplay();
}


GLfloat Display::GetDisplayHeight() {
    throw "Virtual method";
}

GLfloat Display::GetDisplayWidth() {
    throw "Virtual method";
}

void Display::SwapBuffers() {
    throw "Virtual method";
}

void Display::InitGLSurface(unsigned int depthFormat, unsigned int pixelFormat) {
    throw "Virtual method";
}

void Display::setViewPortInPoints(float x, float y, float w, float h) {
    
}

void Display::setScissorInPoints(float x, float y, float w, float h) {
    
}

