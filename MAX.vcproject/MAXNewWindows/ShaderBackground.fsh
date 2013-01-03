//
//  Shader.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

varying vec2 tcoordVarying;

uniform sampler2D colorTexture;
uniform sampler2D colorTexture1;
uniform float alpha;

void main()
{
    //mediump vec4 color = vec4(1.0, 1.0, 1.0, 1.0);//texture2D(colorTexture, tcoordVarying);
    vec4 color = texture2D(colorTexture, tcoordVarying);
//    if(color.r + color.g + color.b < 0.1)
//        discard;

    gl_FragColor = color;//color*alpha + color1*(1.0 - alpha);
}
