//
//  Shader.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

varying mediump vec2 tcoordVarying;

uniform sampler2D colorTexture;
uniform sampler2D colorTexture1;
uniform mediump float alpha;

void main()
{
    mediump vec4 color = texture2D(colorTexture, tcoordVarying);
    mediump vec2 finalCoords = vec2(color.r * 0.9961 + 0.0019, 0.5);
    mediump vec4 colorFinal = texture2D(colorTexture1, finalCoords);
    colorFinal.a = colorFinal.a * alpha * color.a;
    gl_FragColor = colorFinal;
}
