//
//  Shader.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

varying vec2 tcoordVarying;

uniform sampler2D colorTexture;

void main()
{
    gl_FragColor = texture2D(colorTexture, tcoordVarying);
}
