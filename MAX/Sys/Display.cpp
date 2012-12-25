//
//  Display.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#include "Display.h"
#include "ios_display.h"

Display * _display = nullptr;

Display * Display::currentDisplay() {
//    if (!_current) {
//#ifdef TARGET_OS_IPHONE
//        _current = new iosDisplay();
//#else
//        
//#endif
//    }
    
    //
    if(!_display)
        _display = new iosDisplay();
    return _display;
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

void Display::SetViewPortInPoints(float x, float y, float w, float h) {
    
}

void Display::SetScissorInPoints(float x, float y, float w, float h) {
    
}

void Display::SetPinchDelegate(DisplayPinchDelegate *delegate)
{}

