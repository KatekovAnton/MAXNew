//
//  MAXUnitObject.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXUnitObject.h"
#include "MAXUnitMaterial.h"
#include "MAXUnitRenderObject.h"
#include "Shader.h"

const double fireTIme = 0.5;

MAXUnitObject::MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material)
{
    _renderAspect = renderObject;
    _material = material;
    
}

MAXUnitObject::~MAXUnitObject()
{
    delete _renderAspect;
    delete _material;
}

void MAXUnitObject::AfterUpdate()
{
    bodyRenderMatrix = CalculateBodyRenderMatrix();
    headRenderMatrix = CalculateHeadRenderMatrix();
}

GLKMatrix4 MAXUnitObject::CalculateBodyRenderMatrix()
{
//    float cellx = 0;
//    float celly = 1;
//    GLKMatrix4 rt = GLKMatrix4MakeTranslation((cellx - 112/2) + 0.5, ((-1*celly - 1) + 112/2) + 0.5, 0);
    GLKMatrix4 transform = GetTransformMatrix();
    
    FrameInfo i = _material->frames[_material->index];
    scalex = i.rect.size.width/64.0;
    scaley = i.rect.size.height/64.0;
    
    deltax = 0;//i.centerDelta.x/64.0;
    deltay = 0;//i.centerDelta.y/64.0;
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(scale, translate);
    
    return GLKMatrix4Multiply(transform, addtr);
}

GLKMatrix4 MAXUnitObject::CalculateHeadRenderMatrix()
{
    //    float cellx = 0;
    //    float celly = 1;
    //    GLKMatrix4 rt = GLKMatrix4MakeTranslation((cellx - 112/2) + 0.5, ((-1*celly - 1) + 112/2) + 0.5, 0);
    GLKMatrix4 transform = GetTransformMatrix();
    
    FrameInfo i = _material->frames[_material->index];
    scalex = i.rect.size.width/64.0;
    scaley = i.rect.size.height/64.0;
    
    deltax = 0;//i.centerDelta.x/64.0;
    deltay = 0;//i.centerDelta.y/64.0;
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(scale, translate);
    
    return GLKMatrix4Multiply(transform, addtr);
}

RenderObject * MAXUnitObject::GetRenderAspect()
{
    return _renderAspect;
}

Material * MAXUnitObject::GetMaterial()
{
    return _material;
}

void MAXUnitObject::Frame(double time)
{
    _material->DoFrame(time);
    
}

void MAXUnitObject::Draw(Shader *shader)
{
    GLKMatrix4 m1 = bodyRenderMatrix;
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, m1.m);
    GetRenderAspect()->Render(0, GetMaterial());
}

void SetBodyDirection(float azimut)
{}

void SetHeadDirection(float azimut)
{}

void SetIsFireing(bool fire)
{}