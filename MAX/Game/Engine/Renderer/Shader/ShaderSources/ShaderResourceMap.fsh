//
//  Shader.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

varying lowp vec2 tcoordVarying;

uniform sampler2D colorTexture;

void main()
{
    mediump vec4 result = texture2D(colorTexture, tcoordVarying);
    result.a = result.a * 0.7;
    gl_FragColor = result;
}
