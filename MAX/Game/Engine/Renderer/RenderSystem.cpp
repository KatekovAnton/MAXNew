//
//  RenderSystem.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/18/12.
//
//

#include "RenderSystem.h"
#include "Display.h"
#include "CCDirector.h"

RenderSystem::RenderSystem() {
}

void RenderSystem::Init() {
#ifdef TARGET_OS_IPHONE
    _deviceDisplay = Display::currentDisplay();
    _deviceDisplay->InitGLSurface(GL_DEPTH_COMPONENT24_OES, GL_RGB565);
#endif
}

void RenderSystem::InitOpenGL() {
    _deviceDisplay = Display::currentDisplay();
    glViewport(0, 0, _deviceDisplay->GetDisplayWidth(), _deviceDisplay->GetDisplayHeight());
}

void RenderSystem::SetColor(float color){
    _color = color;
}

void RenderSystem::EndFrame() {
    _deviceDisplay->SwapBuffers();
}

Display * RenderSystem::GetDisplay() {
    return _deviceDisplay;
}