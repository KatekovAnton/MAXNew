//
//  Shader.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "Shader.h"
#include "FileManger.h"
#include <stdio.h>

using namespace std;
Shader::Shader(const string& vertexName, const string& fragmentName) {
//#ifdef TARGET_OS_IPHONE
//    LoadShader(vertexName, fragmentName);
//#endif
//#ifdef TARGET_OS_WIN
//	LoadShaderWin(vertexName, fragmentName);
//#endif
	LoadShader(vertexName, fragmentName);
    memset(_shaderUniforms, 0, MAX_UNIFORMS*4);
    glUseProgram(_program);
    _shaderUniforms[UNIFORM_MODEL_MATRIX] = glGetUniformLocation(_program, "modelMatrix");
    _shaderUniforms[UNIFORM_VIEW_MATRIX] = glGetUniformLocation(_program, "viewMatrix");
    
    _shaderUniforms[UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(_program, "projectionMatrix");
    _shaderUniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    _shaderUniforms[UNIFORM_ALPHA] = glGetUniformLocation(_program, "alpha");
    
    _shaderUniforms[UNIFORM_VECTOR1] = glGetUniformLocation(_program, "vector1");
    _shaderUniforms[UNIFORM_VECTOR2] = glGetUniformLocation(_program, "vector2");
    
    
    _shaderUniforms[UNIFORM_COLOR_TEXTURE] = glGetUniformLocation(_program, "colorTexture");
    _shaderUniforms[UNIFORM_COLOR_TEXTURE1] = glGetUniformLocation(_program, "colorTexture1");
    _shaderUniforms[UNIFORM_COLOR_TEXTURE2] = glGetUniformLocation(_program, "colorTexture2");
    _shaderUniforms[UNIFORM_COLOR_TEXTURE3] = glGetUniformLocation(_program, "colorTexture3");
    
    _shaderUniforms[UNIFORM_LIGHTPOSITION] = glGetUniformLocation(_program, "lightPosition");
    
    _shaderUniforms[UNIFORM_FLOATPARAM1] = glGetUniformLocation(_program, "floatParam1");
    _shaderUniforms[UNIFORM_FLOATPARAM2] = glGetUniformLocation(_program, "floatParam2");
    _shaderUniforms[UNIFORM_FLOATPARAM3] = glGetUniformLocation(_program, "floatParam3");
    _shaderUniforms[UNIFORM_FLOATPARAM4] = glGetUniformLocation(_program, "floatParam4");
}

void Shader::SetMatrixValue(int uniformCode, float *value) {
    glUniformMatrix4fv(_shaderUniforms[uniformCode], 1, 0, value);
}

void Shader::SetVector4Value(int uniformCode, float* value) {
    glUniform4fv(_shaderUniforms[uniformCode], 1, value);
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

GLchar* Shader::LoadShaderSource(const char *filename)
{
#ifdef TARGET_OS_WIN
	FILE *file;
	if (fopen_s(&file, filename, "r") != 0)
	{
		std::cout << "Error." << std::endl;
		std::cout << std::endl;
		return NULL;
	}
	struct _stat filestats;
	filestats.st_size = ftell(file);
	GLcharARB *shaderSource = new char[filestats.st_size + 1];
	int bytes = (int)fread(shaderSource, 1, filestats.st_size, file);
	shaderSource[bytes] = 0;
	fclose(file);
	return shaderSource;
#endif
	return NULL;
}

bool Shader::LoadShaderWin(string vertexName, string fragmentName)
{
#ifdef TARGET_OS_WIN
	GLhandleARB objShader;
	GLint result;
	GLsizei len;
	GLcharARB *infoLog;
	GLhandleARB shaderProgram = glCreateProgramObjectARB();

	GLcharARB* source = LoadShaderSource(vertexName.c_str());

	if (source == NULL)
	{
		glDeleteObjectARB(shaderProgram);
		return 0;
	}
	objShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	glShaderSourceARB(objShader, 1, (const GLcharARB **)&source, NULL);
	delete[] source;
	glCompileShaderARB(objShader);
	result = GL_FALSE;
	glGetObjectParameterivARB(objShader, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	glGetObjectParameterivARB(objShader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
	if (len > 1)
	{
		infoLog = (GLcharARB *)malloc(len * sizeof(GLcharARB));
		glGetInfoLogARB(objShader, len, NULL, infoLog);
		std::cout << "Vertex shader log '" << vertexName.c_str() << "':" << std::endl << infoLog << std::endl;
		free((void *)infoLog);
	}
	if (result != GL_TRUE)
	{
		std::cout << "Vertex shader compile error '" << vertexName.c_str() << "'!" << std::endl;
		std::cout << std::endl;
		glDeleteObjectARB(shaderProgram);
		return 0;
	}
	glAttachObjectARB(shaderProgram, objShader);
	glDeleteObjectARB(objShader);
	source = LoadShaderSource(fragmentName.c_str());
	if (source == NULL)
	{
		glDeleteObjectARB(shaderProgram);
		return 0;
	}
	objShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(objShader, 1, (const GLcharARB **)&source, NULL);
	delete[] source;
	glCompileShaderARB(objShader);
	result = GL_FALSE;
	glGetObjectParameterivARB(objShader, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	glGetObjectParameterivARB(objShader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
	if (len > 1)
	{
		infoLog = (GLcharARB *)malloc(len * sizeof(GLcharARB));
		glGetInfoLogARB(objShader, len, NULL, infoLog);
		std::cout << "Fragment shader log '" << fragmentName.c_str() << "':" << std::endl << infoLog << std::endl;
		free((void *)infoLog);
	}
	if (result != GL_TRUE)
	{
		std::cout << "Frament shader compile error '" << fragmentName.c_str() << "'!" << std::endl;
		std::cout << std::endl;
		glDeleteObjectARB(shaderProgram);
		return 0;
	}
	glAttachObjectARB(shaderProgram, objShader);
	glDeleteObjectARB(objShader);
	result = GL_FALSE;
	glLinkProgramARB(shaderProgram);
	glGetObjectParameterivARB(shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &result);
	glGetObjectParameterivARB(shaderProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
	if (len > 1)
	{
		infoLog = (GLcharARB *)malloc(len * sizeof(GLcharARB));
		glGetInfoLogARB(shaderProgram, len, NULL, infoLog);
		std::cout << "Лог шейдерной программы:" << std::endl << infoLog << std::endl;
		free((void *)infoLog);
	}
	if (result != GL_TRUE)
	{
		std::cout << "Ошибка линковки шейдерной программы '" << vertexName.c_str() << "', '" << fragmentName.c_str() << "'!" << std::endl;
		std::cout << std::endl;
		glDeleteObjectARB(shaderProgram);
		return 0;
	}
	_program = shaderProgram;
#endif
}

bool Shader::LoadShader(const string& vertexName, const string& fragmentName)
{
    GLuint vertShader, fragShader;
    
    // Create shader program.
    _program = glCreateProgram();
    // Create and compile vertex shader.
    if (!CompileShader(&vertShader, GL_VERTEX_SHADER, vertexName))
    {
		cout << "Failed to compile vertex shader "<<vertexName.c_str()<<endl;
        return false;
    }
    // Create and compile fragment shader.
    if (!CompileShader(&fragShader, GL_FRAGMENT_SHADER, fragmentName))
    {
        cout << "Failed to compile fragment shader " << fragmentName.c_str() << endl;
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

    GLint logLength = 0;
#ifdef TARGET_OS_IPHONE
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
#endif
#ifdef TARGET_OS_WIN
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
#endif
	GLenum err = glGetError();
	err = err;
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
		if(log[0]!='\0')
			cout << "Shader compile log: \n " << log << endl;
        free(log);
    }
    
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
