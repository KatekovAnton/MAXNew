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
#include "MAXEngine.h"
#include "Geometry.h"
#include "MAXAnimationPrefix.h"

const double fireTIme = 0.15;
const int bodyOffset = 0;
const int headOffset = 8;
const int headFireOffset = 16;

MAXUnitObject::MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material)
:_renderAspect(renderObject),_material(material), fireStartTime(0), changed(true), fireing(false)
{
    bodyIndex = 2;
    headIndex = 9;
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
    
    MAXUnitMaterialFrame bodyframe = _material->frames[bodyIndex];
    float scalex = bodyframe.size.x/64.0;
    float scaley = bodyframe.size.y/64.0;
    
    
    deltax = -(64.0 - bodyframe.size.x)/128.0 - (bodyframe.center.x/64.0);
    deltay = (64.0-bodyframe.size.y)/128.0 + (bodyframe.center.y/64.0);
    
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
    
    return GLKMatrix4Multiply(transform, addtr);
}

GLKMatrix4 MAXUnitObject::CalculateHeadRenderMatrix()
{
    
    GLKMatrix4 transform = GetTransformMatrix();
    
    
    
    MAXUnitMaterialFrame headFrame = _material->frames[headIndex];
    float scalex = headFrame.size.x/64.0;
    float scaley = headFrame.size.y/64.0;
    
    
    deltax = -(64.0 - headFrame.size.x)/128.0 - (headFrame.center.x/64.0);
    deltay = (64.0-headFrame.size.y)/128.0 + (headFrame.center.y/64.0);
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate, scale);
    
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
//    if (fireing && engine->FullTime() - fireStartTime > fireTIme) {
//        SetIsFireing(false);
//    }
}

void MAXUnitObject::Draw(Shader *shader)
{
    _renderAspect->Bind();
    
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, bodyRenderMatrix.m);
    _material->index = bodyIndex;
    _renderAspect->Render(0, _material);
    
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, headRenderMatrix.m);
    _material->index = headIndex;
    _renderAspect->Render(0, _material);
    
    _renderAspect->UnBind();
}

void MAXUnitObject::SetBodyDirection(int state)
{
    bodyIndex = state + bodyOffset;
    changed = true;
}

void MAXUnitObject::SetHeadDirection(int state)
{
    headIndex = state + (fireing?headFireOffset:headOffset);
    changed = true;
}

void MAXUnitObject::SetIsFireing(bool fire)
{
    if(fire == fireing)
        return;
//    if(fire)
//    {
////        int newstate = (bodyIndex) + 1;
////        newstate = newstate % 8;
////        SetBodyDirection(newstate);
//        int newstate = (headIndex - 8) + 1;
//        newstate = newstate % 8;
//        SetHeadDirection(newstate);
//    }
    int state = headIndex - (fireing?headFireOffset:headOffset);
    fireing = fire;
    headIndex = state + (fireing?headFireOffset:headOffset);
    changed = true;
//    if (fireing) {
//        fireStartTime = engine->FullTime();
//    }
}


