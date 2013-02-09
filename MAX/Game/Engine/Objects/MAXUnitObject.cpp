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
#include "MAXUnitConfig.h"

static bool showShadows = false;

MAXUnitObject::MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXUnitConfig* config)
:_renderAspect(renderObject),_material(material), changed(true), fireing(false), _config(config), _lastHeadAnimTime(0)
{
    bodyIndex = 2;
    headIndex = 9;
    
    
    purebodyIndex = 2;
    pureheadIndex = 1;
    
    bodyOffset = 0;
    headOffset = IsHasBody()?8:0;
    headFireOffset = IsHasBody()?16:8;
}

MAXUnitObject::~MAXUnitObject()
{
    delete _renderAspect;
    delete _material;
}

void MAXUnitObject::AfterUpdate()
{
    if(showShadows)
        shadowRenderMatrix = CalculateShadowRenderMatrix();
    bodyRenderMatrix = CalculateBodyRenderMatrix();
    headRenderMatrix = CalculateHeadRenderMatrix();
}

CCPoint MAXUnitObject::CalculateAirOffset()
{
    return CCPoint(0, 0);
}

GLKMatrix4 MAXUnitObject::CalculateShadowRenderMatrix()
{
    GLKMatrix4 transform = GetTransformMatrix();
    
    MAXUnitMaterialFrame shadowFrame = _material->shadowframes[bodyIndex%8];
    float scalex = shadowFrame.size.x/64.0;
    float scaley = shadowFrame.size.y/64.0;
    
    deltax = -(64.0 - shadowFrame.size.x)/128.0 - (shadowFrame.center.x/64.0);
    deltay = (64.0-shadowFrame.size.y)/128.0 + (shadowFrame.center.y/64.0);
    
    CCPoint delta1 = CalculateAirOffset();
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax + delta1.x, deltay + delta1.y, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
    
    return GLKMatrix4Multiply(transform, addtr);
}

GLKMatrix4 MAXUnitObject::CalculateBodyRenderMatrix()
{
    GLKMatrix4 transform = GetTransformMatrix();
    
    MAXUnitMaterialFrame bodyframe = _material->frames[bodyIndex];
    float scalex = bodyframe.size.x/64.0;
    float scaley = bodyframe.size.y/64.0;
    
    deltax = -(64.0 - bodyframe.size.x)/128.0 - (bodyframe.center.x/64.0);
    deltay = (64.0-bodyframe.size.y)/128.0 + (bodyframe.center.y/64.0);
    
    CCPoint delta1 = CalculateAirOffset();
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax + delta1.x, deltay + delta1.y, 0);
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
    if (_config->_isAnimHead) {
        _lastHeadAnimTime+=time;
        if (_lastHeadAnimTime>0.2) {
            _lastHeadAnimTime-=0.2;
            int newHeadOffset = headIndex + 1;
            if (newHeadOffset == _material->frameCount) {
                newHeadOffset = headOffset;
            }
            newHeadOffset-=headOffset;
            SetHeadDirection(newHeadOffset);
        }
    }
}

bool MAXUnitObject::CanFire() const
{
    return _config->_isAbleToFire;
}

bool MAXUnitObject::IsSingleFire() const
{
    return _material->frameCount <= 24;
}

bool MAXUnitObject::IsHasBody() const
{
    return _material->frameCount > 16 && !_config->_isAnimBase;
}

void MAXUnitObject::Draw(Shader *shader)
{
    _renderAspect->Bind();
    _material->ApplyPalette(shader);
    if(showShadows)
    {
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, shadowRenderMatrix.m);
        _material->index = bodyIndex;
        _renderAspect->RenderShadow(0, _material);
    }
    if (IsHasBody())
    {
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, bodyRenderMatrix.m);
        _material->index = bodyIndex;
        _renderAspect->Render(0, _material);
    }
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, headRenderMatrix.m);
    _material->index = headIndex;
    _renderAspect->Render(0, _material);
    
    _renderAspect->UnBind();
}

void MAXUnitObject::SetBodyDirection(int state)
{
    purebodyIndex = state;
    bodyIndex = state + bodyOffset;
    if (!IsHasBody())
        SetHeadDirection(state);
    changed = true;
}

void MAXUnitObject::SetHeadDirection(int state)
{
    pureheadIndex = state;
    headIndex = state + ((_config->_isAnimHead)?(headOffset):(fireing?headFireOffset:headOffset));
    changed = true;
}

void MAXUnitObject::SetIsFireing(bool fire, bool ligthFrame)
{
    if(!_config->_isAbleToFire)
        return;
    
    int offset = fire?headFireOffset:headOffset;
    offset+=(ligthFrame&&!IsSingleFire())?8:0;
   
    headIndex = pureheadIndex + offset;
    fireing = fire;
    changed = true;
}


