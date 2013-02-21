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


#ifdef  TARGET_OS_IPHONE
    #define GLES2    <OpenGLES/ES2/gl.h>
    #define Glext    <OpenGLES/ES2/glext.h>
	#include GLES2
#endif
#ifdef TARGET_OS_WIN
	#include <windows.h>
	#include "glew.h"
	#include "glext.h"
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

class RenderSystem {
    Display             *_deviceDisplay;
    float               _color;
public:
    RenderSystem();
    
    Display * GetDisplay();
    
    
    void Init();
    void InitOpenGL();
    void InitCocos2Dx();
    
    void Draw();
    void EndFrame();
    void SetColor(float color);
};

#endif /* defined(__TerminalExtraction__RenderSystem__) */
