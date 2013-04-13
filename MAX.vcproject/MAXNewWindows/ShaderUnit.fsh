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
    vec4 color = texture2D(colorTexture, tcoordVarying);
	vec2 finalCoords = vec2(color.r - 0.0019, 0.5);
	vec4 colorFinal = texture2D(colorTexture1, finalCoords);
	//colorFinal = colorFinal * alpha;
	gl_FragColor = colorFinal;
}
