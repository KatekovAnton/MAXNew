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

enum
{
    MAXUnitObjectTypeSingle,
    MAXUnitObjectTypeTurreted
} typedef MAXUnitObjectType;

class MAXUnitObject : public PivotObject {
    
    MAXUnitRenderObject        *_renderAspect;//own
    MAXUnitMaterial            *_material;//own
    
    int bodyIndex;
    int headIndex;
    
    GLKMatrix4 bodyRenderMatrix;
    GLKMatrix4 headRenderMatrix;
    
public:
    
    float scalex;
    float scaley;
    
    float deltax;
    float deltay;
    
    MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material);
    ~MAXUnitObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual void Draw(Shader *shader);
    virtual void Frame(double time);
    virtual void AfterUpdate();
    
    
    GLKMatrix4 CalculateBodyRenderMatrix();
    GLKMatrix4 CalculateHeadRenderMatrix();
    
    void SetBodyDirection(float azimut);
    void SetHeadDirection(float azimut);
    void SetIsFireing(bool fire);
    
};

#endif /* defined(__MAX__MAXUnitObject__) */
