//
//  Shader.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#ifndef __TerminalExtraction__Shader__
#define __TerminalExtraction__Shader__

#include <iostream>

#include "Geometry.h"

using namespace std;

class Shader {
    GLuint      _program;
    GLuint      *_shaderUniforms;
    
    bool LoadShader(string vertexName, string fragmentName);
    bool CompileShader(GLuint *shader, GLenum type, string file);
    bool LinkProgram(GLuint program);
    bool ValidateProgram(GLuint program);
    
public:
    
    Shader(string vertexName, string fragmentName);
    void SetMatrixValue(int uniformCode, float *value);
    void SetFloatValue(int uniformCode, float value);
    
    GLuint  GetProgram();
    
    GLuint * GetShaderUniforms();
};

#endif /* defined(__TerminalExtraction__Shader__) */
