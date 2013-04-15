//
//  Shader.vsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

attribute vec4 position;
attribute vec2 tcoord;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

varying vec2 tcoordVarying;

void main()
{
    tcoordVarying = tcoord;
    gl_Position = (projectionMatrix * (viewMatrix * position));
}