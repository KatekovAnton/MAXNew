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
uniform sampler2D colorTexture3;
uniform float alpha;

//texture tiles size w
uniform float floatParam1;// = 1.0/20.0;
//texture tiles size h
uniform float floatParam2;// = 1.0/14.0;

//mapw
uniform float floatParam3;// = 112.0;
//maph
uniform float floatParam4;// = 112.0;

void main()
{
    vec2 tcoordProcessed = tcoordVarying;
    vec4 color = texture2D(colorTexture, tcoordProcessed);
    float index = color.r * 255.0 * 255.0 + color.g * 255.0;


    float _y = floor(index * floatParam1);
    float _x = index - _y / floatParam1;
    
    
    float ccx = fract(tcoordVarying.x * floatParam3);
    float ccy = fract(tcoordVarying.y * floatParam4);
    
    
    vec2 coords = vec2(1.0, 1.0);
    coords.x = (_x + ccx) * floatParam1;
    coords.y = (_y + ccy) * floatParam2;
    
    
    vec4 color1 = texture2D(colorTexture1, coords);

    vec2 finalCoords = vec2(color1.r, 0);
    
    vec4 finalColor = texture2D(colorTexture3, finalCoords);
    gl_FragColor = finalColor;
}
