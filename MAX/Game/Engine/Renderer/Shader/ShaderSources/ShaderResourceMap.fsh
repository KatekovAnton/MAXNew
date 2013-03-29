//
//  Shader.fsh
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

varying highp vec2 tcoordVarying;

uniform sampler2D colorTexture;

void main()
{
    gl_FragColor = vec4(tcoordVarying.x, tcoordVarying.y, 1.0, 1.0);//texture2D(colorTexture, tcoordVarying);
}
