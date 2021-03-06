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
#define MAXBRIDGESCALE  1.1
#define MAXELAPSEDBRIDGESCALE  0.05
#define MAXBRIDGELIFTSPEED 5.0
#define VERTICALSPEED   1.0

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
:MAXObject(config), _renderAspect(renderObject),_material(material), _framesChanged(true), fireing(false), _lastHeadAnimTime(0), _delegate_w(NULL), bodyIndex(0), headIndex(0), purebodyIndex(0), pureheadIndex(0), _connectorsChanged(false), _lowMatrix(2)
{
    
    _needAirOffset = config->_isPlane;
    _needShipOffset = (config->_isShip);
    _airOffsetMult = 1.0;
    _needShadow = false;
    _animRunned = false;
    _bridgeLiftPhase = 0;
    _highLevel = (config->_bLevel >= OBJECT_LEVEL_OVERGROUND);
    if (config->_isPlane)
        _altitude = 1.0;
    else
        _altitude = 0.0;
    _altitudeChange = ALTITUDE_CHANGE_NONE;
    
    _random = nextDoubleMax(1000);
    _playerId = 0;
    _bridgeScale = 1.0;
    _bridgeLiftDirectionUp = false;
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
    if (params_w->_isUnderwater) 
        headFireOffset = 0;
    
    SetHeadDirection(0);
}

MAXUnitObject::~MAXUnitObject()
{
    delete _renderAspect;
}

void MAXUnitObject::OnPositionChanged()
{
    _framesChanged = true;
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
    _connectorShadowMatrices.clear();
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
    if (_material->_shadowframeCount == 0) 
        return;
    
    for (int i = 0; i < _connectorFrames.size(); i++)
    {
        int frame = _connectorFrames[i];
        
        GLKMatrix4 transform = GetTransformMatrix();
        
        MAXUnitMaterialFrame bodyframe = _material->shadowframes[frame];
        float scalex = bodyframe.size.x/64.0;
        float scaley = bodyframe.size.y/64.0;
        
        float deltax = -(64.0 - bodyframe.size.x)/128.0 - (bodyframe.center.x/64.0);
        float deltay = (64.0-bodyframe.size.y)/128.0 + (bodyframe.center.y/64.0);
        GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
        GLKMatrix4 translate = GLKMatrix4MakeTranslation(deltax, deltay, 0);
        GLKMatrix4 addtr = GLKMatrix4Multiply(translate,scale);
        
        _connectorShadowMatrices.push_back(GLKMatrix4Multiply(transform, addtr));
    }
}

void MAXUnitObject::LastUpdate(bool low)
{
    if (!GetIsOnScreen())
        return;
    _framesChanged = _framesChanged || _needShipOffset || _needAirOffset;
    if (low)
    {
        if (_framesChanged || _lowMatrix != 1)
        {
            _bodyRenderMatrix = CalculateLowRenderMatrix();
            _lowMatrix = 1;
        }
    }
    else
    {
        if (_framesChanged || _lowMatrix != 0)
        {
            if (showShadows && params_w->_haveShadow)
                _shadowRenderMatrix = CalculateShadowRenderMatrix();
            if (params_w->_hasHead)
                _bodyRenderMatrix = CalculateBodyRenderMatrix();
            _headRenderMatrix = CalculateHeadRenderMatrix();
            _lowMatrix = 0;
        }
        UpdateConnectors();
    }
    _framesChanged = false;
}

GLKVector2 MAXUnitObject::CalculateAirOffset() const
{
    double elapsedTime = engine->FullTime() - GetSceneLocationTime() + _random;
    double loopTime = elapsedTime - (int)(elapsedTime/(double)LOOPTIME) * LOOPTIME;
    float dt = loopTime/PLANESINGLEANIM;
    int deltaPhase = (int)dt;
    
    float deltaMove = _airOffsetMult * ((deltaPhase%2==0)?dt-floorf(dt):1.0 - dt+floorf(dt));
    
    return GLKVector2Make(planeOffsets[deltaPhase/2].x * deltaMove * _altitude,
                          planeOffsets[deltaPhase/2].y * deltaMove * _altitude);
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
  
    deltax -= (1.0-_bridgeScale)*3;
    deltay += (1.0-_bridgeScale)*3;
    
    GLKMatrix4 scale = GLKMatrix4MakeScale(scalex, scaley, 1);
    GLKMatrix4 translate;
    if (_needAirOffset)
    {
        GLKVector2 offset = CalculateAirOffset();
        translate = GLKMatrix4MakeTranslation(deltax + planeShadowOffset.x * _altitude + offset.x,
                                              deltay + planeShadowOffset.y * _altitude + offset.y, 0);
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
    _bridgeLiftDirectionUp = true;
    _animRunned = true;
}

void MAXUnitObject::DropBridge()
{
    if (!params_w->_isBridge)
        return;
    _bridgeLiftDirectionUp = false;
    _animRunned = true;
}

void MAXUnitObject::TakeOff()
{
    if (!params_w->_isPlane)
        return;
    _altitudeChange = ALTITUDE_CHANGE_TAKE_OFF;
    _currentLevel = params_w->_bLevel;
}

void MAXUnitObject::Landing()
{
    if (!params_w->_isPlane)
        return;
    _altitudeChange = ALTITUDE_CHANGE_LANDING;
    _currentLevel = OBJECT_LEVEL_UNITS;
}

void MAXUnitObject::LiftBridgeInstantly()
{
    if (!params_w->_isBridge)
        return;
    _animRunned = false;
    _bridgeLiftPhase = (MAXBRIDGESCALE - 1.0) / MAXELAPSEDBRIDGESCALE;
    _bridgeScale = 1.0 + _bridgeLiftPhase * MAXELAPSEDBRIDGESCALE;
}

void MAXUnitObject::LandInstantly()
{
    _altitude = 0.0;
    _altitudeChange = ALTITUDE_CHANGE_NONE;
    _currentLevel = OBJECT_LEVEL_UNITS;
}

void MAXUnitObject::StealthActivated()
{
    if (params_w->_isUnderwater)
    {
        SetBodyOffset(8);
    }
    printf("StealthActivated\n");
}

void MAXUnitObject::StealthDeactivated()
{
    if (params_w->_isUnderwater)
    {
        SetBodyOffset(8);
    }
    printf("StealthDeactivated\n");
}

void MAXUnitObject::Frame(double time)
{
    if (params_w->_isBridge && _animRunned)
    {
        _framesChanged = true;
        if (_bridgeLiftDirectionUp)
        {
            _bridgeLiftPhase += time * MAXBRIDGELIFTSPEED;
            if (_bridgeLiftPhase > (MAXBRIDGESCALE - 1.0) / MAXELAPSEDBRIDGESCALE)
            {
                _animRunned = false;
                _bridgeLiftPhase = (MAXBRIDGESCALE - 1.0) / MAXELAPSEDBRIDGESCALE;
            }
        }
        else
        {
            _bridgeLiftPhase -= time * MAXBRIDGELIFTSPEED;
            if (_bridgeLiftPhase < 0)
            {
                _animRunned = false;
                _bridgeLiftPhase = 0;
            }
        }
        _bridgeScale = 1.0 + _bridgeLiftPhase * MAXELAPSEDBRIDGESCALE;
    }
    
    if (_altitudeChange != ALTITUDE_CHANGE_NONE)
    {
        _framesChanged = true;
        if (_altitudeChange == ALTITUDE_CHANGE_TAKE_OFF)
        {
            _altitude += time * VERTICALSPEED;
            if (_altitude > 1.0)
            {
                _altitude = 1.0;
                _altitudeChange = ALTITUDE_CHANGE_NONE;
            }
        }
        else
        {
            _altitude -= time * VERTICALSPEED;
            if (_altitude < 0.0)
            {
                _altitude = 0.0;
                _altitudeChange = ALTITUDE_CHANGE_NONE;
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
    return params_w->_pBulletType != 4;
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
    
    if(_needShadow && _material->_shadowframeCount>0)
    {
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, _shadowRenderMatrix.m);
        shader->SetFloatValue(UNIFORM_ALPHA, SHADOWALPHA);
        _material->index = bodyIndex;
        _material->ApplyShadowLod(0, shader);
        _renderAspect->RenderShadow(0, _material);
    }
    shader->SetFloatValue(UNIFORM_ALPHA, 1.0);
    if (IsHasBody())
    {
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, _bodyRenderMatrix.m);
        _material->index = bodyIndex;
        _material->ApplyLod(0, shader);
        _renderAspect->Render(0, _material);
        DrawConnectors(shader);
    }
    
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, _headRenderMatrix.m);
    _material->index = headIndex;
    _material->ApplyLod(0, shader);
    _renderAspect->Render(0, _material);
    if (!IsHasBody())
        DrawConnectors(shader);
    
   
    
    //_renderAspect->UnBind();
}

void MAXUnitObject::DrawConnectorShadows(Shader* shader)
{
    shader->SetFloatValue(UNIFORM_ALPHA, SHADOWALPHA);
    for (int i = 0; i < _connectorFrames.size(); i++) {
        GLKMatrix4 matr = _connectorShadowMatrices[i];
        int frame = _connectorFrames[i];
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, matr.m);
        _material->index = frame;
        _material->ApplyShadowLod(0, shader);
        _renderAspect->RenderShadow(0, _material);
    }
}

void MAXUnitObject::DrawConnectors(Shader* shader)
{
    DrawConnectorShadows(shader);
    shader->SetFloatValue(UNIFORM_ALPHA, 1.0);
    for (int i = 0; i < _connectorFrames.size(); i++) {
        GLKMatrix4 matr = _connectorMatrices[i];
        int frame = _connectorFrames[i];
        shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, matr.m);
        _material->index = frame;
        _material->ApplyLod(0, shader);
        _renderAspect->Render(0, _material);
    }
}

void MAXUnitObject::DrawLow(Shader *shader)
{
   // _renderAspect->Bind();
    if (engine->_applyedPaletteIndex != _playerId) {
        engine->_applyedPaletteCount ++;
        engine->_applyedPaletteIndex = _playerId;
        shader->SetVector4Value(UNIFORM_VECTOR1, (float*)&playerColor);
    }
    
       
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, _bodyRenderMatrix.m);
    _renderAspect->Render(0, _material);
    
    //_renderAspect->UnBind();
}

void MAXUnitObject::SetBodyDirection(int state)
{
    purebodyIndex = state;
    bodyIndex = state + bodyOffset;
    if (!IsHasBody())
        SetHeadDirection(state);
    _framesChanged = true;
}

void MAXUnitObject::SetBodyOffset(int offset)
{
    if (_material->_frameCount == 1) 
        return;
    
    bodyOffset = offset;
    bodyIndex = purebodyIndex + bodyOffset;
    if (!IsHasBody()) 
        headIndex = pureheadIndex + ((params_w->_isAnimatedHead)?(headOffset):(fireing?headFireOffset:headOffset)) + bodyOffset;
    _framesChanged = true;
}

void MAXUnitObject::SetHeadDirection(int state)
{
    pureheadIndex = state;
    headIndex = state + ((params_w->_isAnimatedHead)?(headOffset):(fireing?headFireOffset:headOffset)) + bodyOffset;
    _framesChanged = true;
}

void MAXUnitObject::SetIsFireing(bool fire, bool ligthFrame)
{
    if(!params_w->_isAbleToFire)
        return;
    
    int offset = fire?headFireOffset:headOffset;
    offset+=(ligthFrame&&!IsSingleFire())?8:0;
    if (!params_w->_isUnderwater) 
        headIndex = pureheadIndex + offset;
    fireing = fire;
    _framesChanged = true;
}


