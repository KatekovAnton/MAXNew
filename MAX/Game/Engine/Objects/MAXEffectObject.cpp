//
//  MAXEffectObject.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXEffectObject.h"
#include "MAXUnitMaterial.h"
#include "MAXUnitRenderObject.h"
#include "Shader.h"
#include "MAXEngine.h"

MAXEffectObject::MAXEffectObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXObjectConfig* config)
:MAXObject(config), _currentFrame(0), _frameCount(material->_frameCount), _material(material), _renderAspect(renderObject)
{
    
}

MAXEffectObject::~MAXEffectObject()
{
    delete _renderAspect;
}

void MAXEffectObject::SetEffectFrame(int frame)
{
    _currentFrame = frame;
}

GLKMatrix4 MAXEffectObject::CalculateBodyRenderMatrix()
{
    GLKMatrix4 transform = GetTransformMatrix();
    
    MAXUnitMaterialFrame bodyframe = _material->frames[_currentFrame];
    float scalex = bodyframe.size.x/64.0;
    float scaley = bodyframe.size.y/64.0;
    
    float deltax = -(64.0 - bodyframe.size.x)/128.0 - (bodyframe.center.x/64.0);
    float deltay = (64.0-bodyframe.size.y)/128.0 + (bodyframe.center.y/64.0);
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
    
    return GLKMatrix4Multiply(transform, addtr);
}

void MAXEffectObject::HasBeenLocatedToScene()
{
    MAXObject::HasBeenLocatedToScene();
    _material->time = GetSceneLocationTime();
}

void MAXEffectObject::Frame(double time)
{
    _material->DoFrame(time);
    if (_material->_autoAnimated) 
    {
        _currentFrame = (engine->FullTime() - GetSceneLocationTime())/0.1;
        _currentFrame = _currentFrame % _material->_frameCount;
    }
}

void MAXEffectObject::Draw(Shader *shader)
{
    //_renderAspect->Bind();
    
    if (engine->_applyedPaletteIndex != -200) {
        engine->_applyedPaletteCount ++;
        engine->_applyedPaletteIndex = -200;
        _material->ApplyPalette(shader, _playerPalette_w);
    }
    
    
    
    shader->SetFloatValue(UNIFORM_ALPHA, 1.0);
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, bodyRenderMatrix.m);
    _material->index = _currentFrame;
    _material->ApplyLod(0, shader);
    _renderAspect->Render(0, _material);
    
    //_renderAspect->UnBind();
}

void MAXEffectObject::DrawLow(Shader *shader)
{
    
}

void MAXEffectObject::LastUpdate(bool low)
{
    if (!GetIsOnScreen())
        return;
    if (!low || _drawInLowLod)
        bodyRenderMatrix = CalculateBodyRenderMatrix();
}