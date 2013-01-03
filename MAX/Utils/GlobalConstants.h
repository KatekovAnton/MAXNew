//
//  GlobalConstants.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/20/12.
//
//

#ifndef TerminalExtraction_GlobalConstants_h
#define TerminalExtraction_GlobalConstants_h

#ifdef  TARGET_OS_IPHONE
    #define GLES2    <OpenGLES/ES2/gl.h>
    #define Glext    <OpenGLES/ES2/glext.h>
	#include GLES2
#endif
#ifdef TARGET_OS_WIN
	#include <windows.h>
	#include "glew.h"
	#include "glext.h"
	#include "wchar.h"
	#include <GL/gl.h>
	#include <GL/glu.h>

	#define __inline__ inline
#endif

#define GCCHECK_GL_ERROR_DEBUG() \
        GLenum __error = glGetError(); \
        if(__error)  \
            printf("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); 

typedef struct _vertexStruct
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tcoord[2];
} vertexStruct;


typedef struct _vertexAnimStruct
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tcoord[2];
    GLint indices[3];
    GLfloat coefficients[3];
} vertexAnimStruct;

enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_TCOORD,
    ATTRIB_INDICES,
    ATTRIB_COEFFICIENTS,
    NUM_ATTRIBUTES
};

#define MAX_UNIFORMS 256

// Uniform index.
enum
{
    UNIFORM_MODEL_MATRIX,
    UNIFORM_VIEW_MATRIX,
    UNIFORM_PROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_COLOR_TEXTURE,
    UNIFORM_ALPHA,
    UNIFORM_VECTOR1,
    UNIFORM_VECTOR2,
    UNIFORM_COLOR_TEXTURE1,
    UNIFORM_LIGHTPOSITION,
    UNIFORM_COLOR_TEXTURE2,
    UNIFORM_COLOR_TEXTURE3,
    UNIFORM_FLOATPARAM1,
    UNIFORM_FLOATPARAM2,
    UNIFORM_FLOATPARAM3,
    UNIFORM_FLOATPARAM4,
    NUM_UNIFORMS
};




#endif
