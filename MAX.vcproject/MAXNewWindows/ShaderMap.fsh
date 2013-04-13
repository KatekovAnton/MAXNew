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
	vec4 color1 = texture2D(colorTexture, tcoordVarying);
	vec2 finalCoords = vec2(color1.r, 0);
	gl_FragColor = texture2D(colorTexture3, finalCoords);
}
