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
#include "cocos2d.h"

class MAXUnitRenderObject;
class MAXUnitMaterial;

using namespace cocos2d;

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
    bool fireing;
    double fireStartTime;
    
    bool changed;
    
    GLKMatrix4 bodyRenderMatrix;
    GLKMatrix4 headRenderMatrix;
    
public:
    
    bool GetFireing() const {return fireing;};
    
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
    
    void SetBodyDirection(int state);
    void SetHeadDirection(int state);
    void SetIsFireing(bool fire);
    
    static GLKMatrix4 MatrixForCell(const CCPoint& cell);
    static int CalculateImageIndex(const CCPoint& cellLocation, const CCPoint& cellTarget);
    
};

#endif /* defined(__MAX__MAXUnitObject__) */
