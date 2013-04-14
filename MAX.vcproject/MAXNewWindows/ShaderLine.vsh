//
//  Shader.vsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

attribute vec4 position;

uniform mat4 projectionMatrix;

void main()
{
    vec4 result = position;
    result.z = 0.5;
    result.w = 0.5;
    gl_Position = projectionMatrix * result;
}