//
//  ShaderPostQuad.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

uniform sampler2D colorTexture;

varying mediump vec2 tcoordVarying;

void main()
{
    mediump vec4 colortexturevalue = texture2D(colorTexture, tcoordVarying);
    gl_FragColor = vec4(colortexturevalue.xyz, 0.0);
}