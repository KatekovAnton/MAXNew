//
//  MathKit.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/30/12.
//
//

#ifndef TerminalExtraction_MathKit_h
#define TerminalExtraction_MathKit_h

#include "GlobalConstants.h"

#ifdef  TARGET_OS_IPHONE
    #define GLES2    <OpenGLES/ES2/gl.h>
    #define Glext    <OpenGLES/ES2/glext.h>
#include GLES2
#include Glext
#endif 
#ifdef TARGET_OS_WIN
#include <windows.h>
#include "glew.h"
#include "glext.h"
#include "wchar.h"
#include <GL/gl.h>
#include <GL/glu.h>
#endif

struct __GPoint2D {
    GLfloat x;
    GLfloat y;
};

struct __GSize2D {
    GLfloat height;
    GLfloat width;
};

struct __GRect2D {
    struct __GPoint2D   origin;
    struct __GSize2D    size;
};


typedef struct __GPoint2D GPoint2D;
typedef struct __GRect2D GRect2D;
typedef struct __GSize2D GSize2D;


static __inline__ GRect2D GRect2DMake(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GRect2D rect;
    rect.origin.x = x;
    rect.origin.y = y;
    rect.size.width = width;
    rect.size.height = height;
    return rect;
};

#include "MyMath.h"
#include "GLKMath.h"

//http://www.opengl.org/wiki/GluProject_and_gluUnProject_code

#endif
