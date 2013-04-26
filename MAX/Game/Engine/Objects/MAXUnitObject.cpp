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
#include "MyRandom.h"
#include "Texture.h"

static bool showShadows = true;

#define PLANESINGLEANIM 0.25
#define LOOPTIME        4.0 // 8 * 2 * PLANESINGLEANIM
#define OFFSETSCALE     0.05
#define OFFSETSCALESHIP 0.01
#define MAXBRIDGESCALE  1.05
#define MAXELAPSEDBRIDGESCALE  0.05

GLKVector2 planeOffsets[] = {
    {0 * OFFSETSCALE,       -1 * OFFSETSCALE},
    {0.85f * OFFSETSCALE,   -0.85f * OFFSETSCALE},
    {1 * OFFSETSCALE,       0 * OFFSETSCALE},
    {0.85 * OFFSETSCALE,    0.85 * OFFSETSCALE},
    {0 * OFFSETSCALE,       1 * OFFSETSCALE},
    {-0.85 * OFFSETSCALE,   0.85 * OFFSETSCALE},
    {-1 * OFFSETSCALE,      0 * OFFSETSCALE},
    {-0.85 * OFFSETSCALE,   -0.85 * OFFSETSCALE}};

GLKVector2 shipOffsets[] = {
    {0 * OFFSETSCALESHIP,       -1 * OFFSETSCALESHIP},
    {0.85f * OFFSETSCALESHIP,   -0.85f * OFFSETSCALESHIP},
    {1 * OFFSETSCALESHIP,       0 * OFFSETSCALESHIP},
    {0.85 * OFFSETSCALESHIP,    0.85 * OFFSETSCALESHIP},
    {0 * OFFSETSCALESHIP,       1 * OFFSETSCALESHIP},
    {-0.85 * OFFSETSCALESHIP,   0.85 * OFFSETSCALESHIP},
    {-1 * OFFSETSCALESHIP,      0 * OFFSETSCALESHIP},
    {-0.85 * OFFSETSCALESHIP,   -0.85 * OFFSETSCALESHIP}};

GLKVector2 planeShadowOffset = {1.0, -1.0};

MAXUnitObject::MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXObjectConfig* config)
:MAXObject(config), _renderAspect(renderObject),_material(material), changed(true), fireing(false), _lastHeadAnimTime(0), _delegate_w(NULL), bodyIndex(0), headIndex(0), purebodyIndex(0), pureheadIndex(0), _connectorsChanged(false)
{
    
    _needAirOffset = config->_isPlane;
    _needShipOffset = config->_isShip && !config->_isBuilding;
    _airOffsetMult = 1.0;
    _needShadow = false;
    _animRunned = false;
    _highLevel = (config->_bLevel >= OBJECT_LEVEL_OVERGROUND);
    
    _random = nextDoubleMax(1000);
    _playerId = 0;
    _bridgeScale = 1.0;
    _bridgeAnimDirection = false;
    bodyOffset = 0;
    headOffset = params_w->_isBuilding?((IsHasBody())?1:0):(IsHasBody()?8:0);
    if(IsHasBody())
        headFireOffset = params_w->_isBuilding?9:16;
    else
    {
        if (params_w->_isAmphibious)
            headFireOffset = 16;
        else
            headFireOffset = 8;
    }
    params_w->_isMultifire = (IsHasBody() && _material->_frameCount == 32) || (params_w->_isBuilding && params_w->_pFireType == 3);
    SetHeadDirection(0);
}

MAXUnitObject::~MAXUnitObject()
{
    delete _renderAspect;
}

int MAXUnitObject::FrameForConnectorLocation(MAXUNITOBJECT_CONNECTOR connector) const
{
    return params_w->connectorFrameStart + static_cast<int>(connector);
}

void MAXUnitObject::AddConnector(MAXUNITOBJECT_CONNECTOR connector)
{
    _connectorsChanged = true;
    _connectorFrames.push_back(FrameForConnectorLocation(connector));
}

void MAXUnitObject::RemoveConnectors()
{
    _connectorsChanged = true;
    _connectorFrames.clear();
}

void MAXUnitObject::UpdateConnectors()
{
    if (!_connectorsChanged)
        return;
    _connectorsChanged = false;
    _connectorMatrices.clear();

    for (int i = 0; i < _connectorFrames.size(); i++)
    {
        int frame = _connectorFrames[i];
        
        GLKMatrix4 transform = GetTransformMatrix();
        
        MAXUnitMaterialFrame bodyframe = _material->frames[frame];
        float scalex = bodyframe.size.x/64.0;
        float scaley = bodyframe.size.y/64.0;
        
        float deltax = -(64.0 - bodyframe.size.x)/128.0 - (bodyframe.center.x/64.0);
        float deltay = (64.0-bodyframe.size.y)/128.0 + (bodyframe.center.y/64.0);
        GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
        GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
        GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
        
        _connectorMatrices.push_back(GLKMatrix4Multiply(transform, addtr));
    }
}

void MAXUnitObject::LastUpdate(bool low)
{
    if (!GetIsOnScreen())
        return;
    if (low)
    {
        bodyRenderMatrix = CalculateLowRenderMatrix();
    }
    else
    {
        if (showShadows && params_w->_haveShadow)
            shadowRenderMatrix = CalculateShadowRenderMatrix();
        if (params_w->_hasHead)
            bodyRenderMatrix = CalculateBodyRenderMatrix();
        headRenderMatrix = CalculateHeadRenderMatrix();
        UpdateConnectors();
    }
}

GLKVector2 MAXUnitObject::CalculateAirOffset() const
{
    double elapsedTime = engine->FullTime() - GetSceneLocationTime() + _random;
    double loopTime = elapsedTime - (int)(elapsedTime/(double)LOOPTIME) * LOOPTIME;
    float dt = loopTime/PLANESINGLEANIM;
    int deltaPhase = (int)dt;
    
    float deltaMove = _airOffsetMult * ((deltaPhase%2==0)?dt-floorf(dt):1.0 - dt+floorf(dt));
    
    return GLKVector2Make(planeOffsets[deltaPhase/2].x * deltaMove, planeOffsets[deltaPhase/2].y * deltaMove);
}

GLKVector2 MAXUnitObject::CalculateShipOffset() const
{
    double elapsedTime = engine->FullTime() - GetSceneLocationTime() + _random;
    elapsedTime *= 0.25;
    double loopTime = elapsedTime - (int)(elapsedTime/(double)LOOPTIME) * LOOPTIME;
    float dt = loopTime/PLANESINGLEANIM;
    int deltaPhase = (int)dt;
    
    float deltaMove = (deltaPhase%2==0)?dt-floorf(dt):1.0 - dt+floorf(dt);
    
    return GLKVector2Make(shipOffsets[deltaPhase/2].x * deltaMove, shipOffsets[deltaPhase/2].y * deltaMove);
}

GLKVector2 MAXUnitObject::GetDeltaPosition() const
{
    if (_needAirOffset) 
        return CalculateAirOffset();
    if (_needShipOffset)
        return CalculateShipOffset();
    return MAXObject::GetDeltaPosition();
    
}

GLKMatrix4 MAXUnitObject::CalculateShadowRenderMatrix()
{
    GLKMatrix4 transform = GetTransformMatrix();
    
    MAXUnitMaterialFrame shadowFrame = _material->shadowframes[bodyIndex];
    float scalex = shadowFrame.size.x/64.0;
    float scaley = shadowFrame.size.y/64.0;
    
    float deltax = -(64.0 - shadowFrame.size.x)/128.0 - (shadowFrame.center.x/64.0);
    float deltay = (64.0-shadowFrame.size.y)/128.0 + (shadowFrame.center.y/64.0);
  
    deltax -= (1.0-_bridgeScale)*2;
    deltay += (1.0-_bridgeScale)*2;
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate;
    if (_needAirOffset)
    {
        GLKVector2 offset = CalculateAirOffset();
        translate = GLKMatrix4MakeTranslation(deltax + planeShadowOffset.x + offset.x, deltay + planeShadowOffset.y + offset.y, 0);
    }
    else if (_needShipOffset)
    {
        GLKVector2 offset = CalculateShipOffset();
        translate = GLKMatrix4MakeTranslation(deltax + offset.x, deltay + offset.y, 0);
    }
    else
        translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
    return GLKMatrix4Multiply(transform, addtr);
}

GLKMatrix4 MAXUnitObject::CalculateBodyRenderMatrix()
{
    GLKMatrix4 transform = GetTransformMatrix();
    
    MAXUnitMaterialFrame bodyframe = _material->frames[bodyIndex];
    float scalex = bodyframe.size.x/64.0;
    float scaley = bodyframe.size.y/64.0;
    
    float deltax = -(64.0 - bodyframe.size.x)/128.0 - (bodyframe.center.x/64.0);
    float deltay = (64.0-bodyframe.size.y)/128.0 + (bodyframe.center.y/64.0);
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate;
    if (_needAirOffset)
    {
        GLKVector2 offset = CalculateAirOffset();
        translate = GLKMatrix4MakeTranslation(deltax + offset.x, deltay + offset.y, 0);
    }
    else if (_needShipOffset)
    {
        GLKVector2 offset = CalculateShipOffset();
        translate = GLKMatrix4MakeTranslation(deltax + offset.x, deltay + offset.y, 0);
    }
    else
        translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
    
    return GLKMatrix4Multiply(transform, addtr);
}

GLKMatrix4 MAXUnitObject::CalculateLowRenderMatrix()
{
    GLKMatrix4 transform = GetTransformMatrix();
    float size = params_w->_bSize;
    float deltax = -0.5;
    float deltay = 0.5;
    GLKMatrix4 scale = GLKMatrix4MakeScale(size, size, 1);
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
    
    float deltax = -(64.0 - headFrame.size.x)/128.0 - (headFrame.center.x/64.0);
    float deltay = (64.0-headFrame.size.y)/128.0 + (headFrame.center.y/64.0);
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex * _bridgeScale, scaley * _bridgeScale, 1);
    GLKMatrix4 translate;
    if (_needAirOffset)
    {
        GLKVector2 offset = CalculateAirOffset();
        translate = GLKMatrix4MakeTranslation(deltax + offset.x, deltay + offset.y, 0);
    }
    else if (_needShipOffset)
    {
        GLKVector2 offset = CalculateShipOffset();
        translate = GLKMatrix4MakeTranslation(deltax + offset.x, deltay + offset.y, 0);
    }
    else
        translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
    GLKMatrix4 addtr = GLKMatrix4Multiply(translate, scale);
    
    return GLKMatrix4Multiply(transform, addtr);
}

GLKMatrix4 MAXUnitObject::CalculateUnitCenterMatrix() const
{
    GLKMatrix4 transform = GetTransformMatrix();
    GLKMatrix4 translate = GLKMatrix4Identity;
    if (_needAirOffset)
    {
        GLKVector2 offset = CalculateAirOffset();
        translate = GLKMatrix4MakeTranslation(offset.x, offset.y, 0);
    }
    else if (_needShipOffset)
    {
        GLKVector2 offset = CalculateShipOffset();
        translate = GLKMatrix4MakeTranslation(offset.x, offset.y, 0);
    }
    
    return GLKMatrix4Multiply(transform, translate);
}

RenderObject * MAXUnitObject::GetRenderAspect()
{
    return _renderAspect;
}

Material * MAXUnitObject::GetMaterial()
{
    return _material;
}

void MAXUnitObject::LiftBridge()
{
    if (!params_w->_isBridge)
        return;
    _bridgeStartAnim = 0;
    _bridgeAnimDirection = true;
    _animRunned = true;
}

void MAXUnitObject::DropBridge()
{
    if (!params_w->_isBridge)
        return;
    _bridgeStartAnim = 0;
    _bridgeAnimDirection = false;
    _animRunned = true;
}

void MAXUnitObject::Frame(double time)
{
    if (params_w->_isBridge && _animRunned)
    {
        _bridgeStartAnim += time*5.0;
        double delta = _bridgeStartAnim;
        if((_bridgeAnimDirection))
        {
            _bridgeScale = 1.0 + delta*MAXELAPSEDBRIDGESCALE;
            if (_bridgeScale>MAXBRIDGESCALE)
            {
                _animRunned = false;
                _bridgeScale = MAXBRIDGESCALE;
            }
        }
        if ((!_bridgeAnimDirection))
        {
            
            _bridgeScale = MAXBRIDGESCALE - delta*MAXELAPSEDBRIDGESCALE;
            if (_bridgeScale<1.0)
            {
                _animRunned = false;
                _bridgeScale = 1.0;
            }
        }
    }
    
    
    _lastHeadAnimTime+=time;
    if (_lastHeadAnimTime>0.05)
        _lastHeadAnimTime=0;
    else
        return;
    
    if (params_w->_isAnimatedHead)
    {
        int newHeadOffset = pureheadIndex + 1;
        if (newHeadOffset == params_w->bodyAnimFrame1 - params_w->bodyAnimFrame0 + 1)
            newHeadOffset = 0;
        SetHeadDirection(newHeadOffset);
    }
    if (_delegate_w->ShouldAnimateBody())
    {
        int newBodyOffset = purebodyIndex + 1;
        if (newBodyOffset == params_w->bodyActiveFrame1 + 1)
            newBodyOffset = params_w->bodyActiveFrame0;
//        if (params_w->_isAllwaysOn)
//            bodyOffset = 0;
        SetBodyDirection(newBodyOffset);
    }
}

bool MAXUnitObject::CanFire() const
{
    return params_w->_isAbleToFire;
}

FIRE_TYPE MAXUnitObject::FireType() const
{
    return (FIRE_TYPE)params_w->_pFireType;
}

bool MAXUnitObject::IsSingleFire() const
{
    return !params_w->_isMultifire;
}

bool MAXUnitObject::IsHasBody() const
{
    return params_w->_hasHead;
}

void MAXUnitObject::Draw(Shader *shader)
{
    //_renderAspect->Bind();
    if (engine->_applyedPaletteIndex != _playerId) {
        engine->_applyedPaletteCount ++;
        engine->_applyedPaletteIndex = _playerId;
        _material->ApplyPalette(shader, *_playerPalette_w);
    }
    
    if(_needShadow)
    {
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, shadowRenderMatrix.m);
        shader->SetFloatValue(UNIFORM_ALPHA, SHADOWALPHA);
        _material->index = bodyIndex;
        _material->ApplyShadowLod(0, shader);
        _renderAspect->RenderShadow(0, _material);
    }
    shader->SetFloatValue(UNIFORM_ALPHA, 1.0);
    if (IsHasBody())
    {
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, bodyRenderMatrix.m);
        _material->index = bodyIndex;
        _material->ApplyLod(0, shader);
        _renderAspect->Render(0, _material);
    }
    
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, headRenderMatrix.m);
    _material->index = headIndex;
    _material->ApplyLod(0, shader);
    _renderAspect->Render(0, _material);
    
    for (int i = 0; i < _connectorFrames.size(); i++) {
        GLKMatrix4 matr = _connectorMatrices[i];
        int frame = _connectorFrames[i];
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, matr.m);
        _material->index = frame;
        _material->ApplyLod(0, shader);
        _renderAspect->Render(0, _material);
    }
    
    //_renderAspect->UnBind();
}

void MAXUnitObject::DrawLow(Shader *shader)
{
   // _renderAspect->Bind();
    if (engine->_applyedPaletteIndex != _playerId) {
        engine->_applyedPaletteCount ++;
        engine->_applyedPaletteIndex = _playerId;
        shader->SetVector4Value(UNIFORM_VECTOR1, (float*)&playerColor);
    }
    
       
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, bodyRenderMatrix.m);
    _renderAspect->Render(0, _material);
    
    //_renderAspect->UnBind();
}

void MAXUnitObject::SetBodyDirection(int state)
{
    purebodyIndex = state;
    bodyIndex = state + bodyOffset;
    if (!IsHasBody())
        SetHeadDirection(state);
    changed = true;
}

void MAXUnitObject::SetBodyOffset(int offset)
{
    if (_material->_frameCount == 1) 
        return;
    
    bodyOffset = offset;
    bodyIndex = purebodyIndex + bodyOffset;
    if (!IsHasBody()) 
        headIndex = pureheadIndex + ((params_w->_isAnimatedHead)?(headOffset):(fireing?headFireOffset:headOffset)) + bodyOffset;
    changed = true;
}

void MAXUnitObject::SetHeadDirection(int state)
{
    pureheadIndex = state;
    headIndex = state + ((params_w->_isAnimatedHead)?(headOffset):(fireing?headFireOffset:headOffset)) + bodyOffset;
    changed = true;
}

void MAXUnitObject::SetIsFireing(bool fire, bool ligthFrame)
{
    if(!params_w->_isAbleToFire)
        return;
    
    int offset = fire?headFireOffset:headOffset;
    offset+=(ligthFrame&&!IsSingleFire())?8:0;
   
    headIndex = pureheadIndex + offset;
    fireing = fire;
    changed = true;
}


