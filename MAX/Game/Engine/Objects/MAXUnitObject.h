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
    
    MAXUnitRenderObject        *_renderAspect;//own
    MAXUnitMaterial            *_material;//own
public:
    
    float scalex;
    float scaley;
    
    float deltax;
    float deltay;
    
    MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material);
    ~MAXUnitObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    
    virtual GLKMatrix4 CalculateRenderMatrix(GLKMatrix4 transform);
    virtual void Frame(double time);
    
    void SetBodyDirection(float azimut);
    void SetHeadDirection(float azimut);
    
};

#endif /* defined(__MAX__MAXUnitObject__) */
