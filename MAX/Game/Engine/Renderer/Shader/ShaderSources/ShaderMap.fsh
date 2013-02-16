//
//  Shader.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

varying highp vec2 tcoordVarying;

uniform sampler2D colorTexture;
uniform sampler2D colorTexture1;
uniform sampler2D colorTexture3;
uniform mediump float alpha;

//texture tiles size w
uniform highp float floatParam1;// = 1.0/20.0;
//texture tiles size h
uniform highp float floatParam2;// = 1.0/14.0;

//mapw
uniform highp float floatParam3;// = 112.0;
//maph
uniform highp float floatParam4;// = 112.0;

void main()
{
//    highp vec2 tcoordProcessed = tcoordVarying;
//    mediump vec4 color = texture2D(colorTexture, tcoordProcessed);
//    mediump float index = color.r * 255.0 * 255.0 + color.g * 255.0;
//
//    mediump float _y = floor(index * floatParam1);  
//    mediump float _x = index - _y / floatParam1;
//    
//    
//    highp float ccx = fract(tcoordVarying.x * floatParam3);
//    highp float ccy = fract(tcoordVarying.y * floatParam4);
//    
//    
//    highp vec2 coords = vec2(1.0, 1.0);
//    coords.x = (_x + ccx) * floatParam1;
//    coords.y = (_y + ccy) * floatParam2;
//    
//    
//    mediump vec4 color1 = texture2D(colorTexture1, coords);
//
//    mediump vec2 finalCoords = vec2(color1.r, 0);
//    
//    mediump vec4 finalColor = texture2D(colorTexture3, finalCoords);
//    gl_FragColor = finalColor;
    mediump vec4 color1 = texture2D(colorTexture, tcoordVarying);
    mediump vec2 finalCoords = vec2(color1.r, 0);
    gl_FragColor = texture2D(colorTexture3, finalCoords);
}
