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
    //mediump vec4 color = vec4(1.0, 1.0, 1.0, 1.0);//texture2D(colorTexture, tcoordVarying);
    mediump vec4 color = texture2D(colorTexture, tcoordVarying);
    
    gl_FragColor = color;//color*alpha + color1*(1.0 - alpha);
}
