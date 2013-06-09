//
//  Display.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/18/12.
//
//

#include "Display.h"
#ifdef TARGET_OS_IPHONE
#include "ios_display.h"
#endif
#ifdef TARGET_OS_WIN
#include "windows_display.h"
#endif

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
#ifdef TARGET_OS_IPHONE
    if(!_display)
    {
        _display = new iosDisplay();
        _display->InitGLSurface(GL_DEPTH_COMPONENT24_OES, GL_RGB565);
    }
    return _display;
#endif
#ifdef TARGET_OS_WIN
	return windows_display::GetSharedDisplay();
#endif
}


GLfloat Display::GetDisplayHeight() {
    throw "Virtual method";
}

GLfloat Display::GetDisplayWidth() {
    throw "Virtual method";
}

GLfloat Display::GetDisplayScale() {
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

