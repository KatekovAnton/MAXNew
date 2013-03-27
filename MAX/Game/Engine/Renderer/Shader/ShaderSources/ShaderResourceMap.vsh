//
//  Shader.vsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

attribute vec4 position;
attribute vec2 tcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

varying highp vec2 tcoordVarying;

void main()
{
    tcoordVarying = vec2(tcoord.x, tcoord.y);
    gl_Position = (projectionMatrix * (viewMatrix * (modelMatrix * position)));
}