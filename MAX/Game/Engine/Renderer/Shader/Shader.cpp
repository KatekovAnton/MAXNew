//
//  Shader.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "Shader.h"
#include "FileManger.h"

Shader::Shader(string vertexName, string fragmentName) {
    LoadShader(vertexName, fragmentName);
    
    _shaderUniforms = new GLuint[MAX_UNIFORMS];
    
    _shaderUniforms[UNIFORM_MODEL_MATRIX] = glGetUniformLocation(_program, "modelMatrix");
    _shaderUniforms[UNIFORM_VIEW_MATRIX] = glGetUniformLocation(_program, "viewMatrix");
    _shaderUniforms[UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(_program, "projectionMatrix");
    _shaderUniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    _shaderUniforms[UNIFORM_COLOR_TEXTURE] = glGetUniformLocation(_program, "colorTexture");
    _shaderUniforms[UNIFORM_ALPHA] = glGetUniformLocation(_program, "alpha");
    
    
    _shaderUniforms[UNIFORM_VECTOR1] = glGetUniformLocation(_program, "vector1");
    _shaderUniforms[UNIFORM_VECTOR2] = glGetUniformLocation(_program, "vector2");
    
    
    _shaderUniforms[UNIFORM_COLOR_TEXTURE1] = glGetUniformLocation(_program, "colorTexture1");
    _shaderUniforms[UNIFORM_LIGHTPOSITION] = glGetUniformLocation(_program, "lightPosition");
}

void Shader::SetMatrixValue(int uniformCode, float *value) {
    glUniformMatrix4fv(_shaderUniforms[uniformCode], 1, 0, value);
}

void Shader::SetFloatValue(int uniformCode, float value) {
    glUniform1f(_shaderUniforms[uniformCode], value);
}

GLuint Shader::GetProgram() {
    return _program;
}

GLuint * Shader::GetShaderUniforms() {
    return _shaderUniforms;
}

bool Shader::LoadShader(string vertexName, string fragmentName) {
    GLuint vertShader, fragShader;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    if (!CompileShader(&vertShader, GL_VERTEX_SHADER, vertexName))
    {
        cout << "Failed to compile vertex shader "<<vertexName<<endl;
        return false;
    }
    
    // Create and compile fragment shader.
    if (!CompileShader(&fragShader, GL_FRAGMENT_SHADER, fragmentName))
    {
        cout << "Failed to compile fragment shader " << fragmentName << endl;
        return false;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(_program, ATTRIB_NORMAL, "normal");
    glBindAttribLocation(_program, ATTRIB_TCOORD, "tcoord");
    glBindAttribLocation(_program, ATTRIB_INDICES, "indices");
    glBindAttribLocation(_program, ATTRIB_COEFFICIENTS, "coefficients");
    
    // Link program.
    if (!LinkProgram(_program))
    {
        cout << "Failed to link program: " << _program << endl;
        
        if (vertShader)
        {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader)
        {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program)
        {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return false;
    }
    
    // Release vertex and fragment shaders.
    if (vertShader)
    {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader)
    {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    return true;
}

bool Shader::CompileShader(GLuint *shader, GLenum type, string file) {
    
    GLint status;
    string *source_str;
    const GLchar *source;
    
    FileManager * manager = FileManager::CreateManager();
    source_str = manager->GetContent(file);
    source = (GLchar *)source_str->c_str();
    //cout << "Shader source code: \n " << source << endl;
    if (!source) {
        cout << "Failed to load shader" << endl;
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        cout << "Shader compile log: \n " << log << endl;
        free(log);
    }
#endif
    
    delete manager;
    delete source_str;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

bool Shader::LinkProgram(GLuint program) {
    
    GLint status;
    glLinkProgram(program);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        cout << "Program link log: \n " << log << endl;
        free(log);
    }
#endif
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0)
        return false;
    
    
    return true;
}

bool Shader::ValidateProgram(GLuint program) {
    
    GLint logLength, status;
    
    glValidateProgram(program);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        cout << "Program validate log: \n " << log << endl;
        free(log);
    }
    
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == 0)
        return false;
    
    
    return true;
    
}
