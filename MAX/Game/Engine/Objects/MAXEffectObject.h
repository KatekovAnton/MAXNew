//
//  MAXEffectObject.h
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXEffectObject__
#define __MAX__MAXEffectObject__

#include <iostream>
#include "MAXObject.h"

class MAXUnitRenderObject;
class MAXUnitMaterial;
class Texture;

class MAXEffectObject : public MAXObject {
    
public:
    
    int _frameCount;
    
    MAXEffectObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXObjectConfig* config);
    
    void SetEffectFrame(int frame);
};

#endif /* defined(__MAX__MAXEffectObject__) */
