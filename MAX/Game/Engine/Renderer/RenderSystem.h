//
//  RenderSystem.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/18/12.
//
//

#ifndef __TerminalExtraction__RenderSystem__
#define __TerminalExtraction__RenderSystem__

#include "GlobalConstants.h"
#include <iostream>

class Display;

class RenderSystem {
    Display             *_deviceDisplay;
    float               _color;
public:
    RenderSystem();
    
    Display * GetDisplay();
    
    
    void Init();
    void InitOpenGL();
    
    void EndFrame();
    void SetColor(float color);
};

#endif /* defined(__TerminalExtraction__RenderSystem__) */
