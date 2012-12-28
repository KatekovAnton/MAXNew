//
//  MAXUnitObject.h
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXUnitObject__
#define __MAX__MAXUnitObject__

#include <iostream>
#include "PivotObject.h"

class MAXUnitRenderObject;
class MAXUnitMaterial;

class MAXUnitObject : public PivotObject {
    
    MAXUnitRenderObject        *_renderAspect;
    MAXUnitMaterial            *_material;
public:
    
    MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material);
    ~MAXUnitObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    
    virtual void Frame(double time);
};

#endif /* defined(__MAX__MAXUnitObject__) */
