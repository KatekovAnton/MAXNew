//
//  Shader.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/3/12.
//
//

#ifndef __TerminalExtraction__Shader__
#define __TerminalExtraction__Shader__

#include <iostream>
#include "Geometry.h"

/*
 void main()
 {
 //координаты по colorTexture дают нам индекс блока(тайла)
 highp vec2 tcoordProcessed = tcoordVarying;
 mediump vec4 color = texture2D(colorTexture, tcoordProcessed);
 mediump float index = color.r * 255.0 * 255.0 + color.g * 255.0;
 
 
 //имея индекс надо получить его положение в тайловой текстуре
 mediump float _y = floor(index * floatParam1);
 mediump float _x = index - _y / floatParam1;
 
 
 //помимо положения в тайловой текстуре необходимо знать
 //точные координаты внутри тайлового блока
 highp float ccx = fract(tcoordVarying.x * floatParam3);
 highp float ccy = fract(tcoordVarying.y * floatParam4);
 
 
 //вычисляем текстурные координвты внутри тайловой текструы
 //имея координаты тайла и координвты внтури тайла, а также
 //зная размер всей тайловой карты
 highp vec2 coords = vec2(1.0, 1.0);
 coords.x = (_x + ccx) * floatParam1;
 coords.y = (_y + ccy) * floatParam2;
 
 
 //цвет в тайловой карте дает нам индекс цвета из палитры
 mediump vec4 color1 = texture2D(colorTexture1, coords);
 
 //по индексу расчитываем координаты в текстуре-палитре
 mediump vec2 finalCoords = vec2(color1.r, 0);
 
 //теперь берем этот цвет из палитры
 mediump vec4 finalColor = texture2D(colorTexture3, finalCoords);
 gl_FragColor = finalColor;
 }
 */

using namespace std;

class Shader {
    GLuint      _program;
    GLuint      _shaderUniforms[MAX_UNIFORMS];
    
    bool LoadShader(const string& vertexName, const string& fragmentName);
	bool LoadShaderWin(string vertexName, string fragmentName);

	GLchar* LoadShaderSource(const char *filename);

    bool CompileShader(GLuint *shader, GLenum type, string file);
    bool LinkProgram(GLuint program, const string& vname, const string& fname);
    bool ValidateProgram(GLuint program);
    
public:
    
    Shader(const string& vertexName, const string& fragmentName);
    void SetMatrixValue(int uniformCode, float *value);
    void SetFloatValue(int uniformCode, float value);
    void SetVector4Value(int uniformCode, float* value);
    
    GLuint  GetProgram();
    
    GLuint * GetShaderUniforms();
};

#endif /* defined(__TerminalExtraction__Shader__) */
