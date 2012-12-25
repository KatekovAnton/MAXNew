//
//  Shader.vsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;
attribute vec2 tcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

varying mediump vec2 tcoordVarying;

void main()
{
    vec3 v = normal;
    tcoordVarying = vec2(tcoord.x, 1.0-tcoord.y);
    gl_Position = (projectionMatrix * (viewMatrix * (modelMatrix * position)));
}