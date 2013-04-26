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
#include "MAXObject.h"
#include "cocos2d.h"
#include "MAXObjectConfig.h"
#include "MAXUnitObjectDelegate.h"


class MAXUnitRenderObject;
class MAXUnitMaterial;
class Texture;

using namespace cocos2d;

enum
{
    MAXUnitObjectTypeSingle,
    MAXUnitObjectTypeTurreted
} typedef MAXUnitObjectType;
/*
 clockwise
 
 1х1
 U, R, D, L
 */
enum
{
    MAXUNITOBJECT_CONNECTOR1 = 0,
    MAXUNITOBJECT_CONNECTOR2 = 1,
    MAXUNITOBJECT_CONNECTOR3 = 2,
    MAXUNITOBJECT_CONNECTOR4 = 3,
    MAXUNITOBJECT_CONNECTOR5 = 4,
    MAXUNITOBJECT_CONNECTOR6 = 5,
    MAXUNITOBJECT_CONNECTOR7 = 6,
    MAXUNITOBJECT_CONNECTOR8 = 7
} typedef MAXUNITOBJECT_CONNECTOR;

class MAXUnitObject : public MAXObject {

    double _random;
    
    MAXUnitRenderObject        *_renderAspect;//own
    MAXUnitMaterial            *_material;//not own
    
    int bodyIndex;
    int headIndex;
    
    int purebodyIndex;
    int pureheadIndex;
    
    bool fireing;
    
    bool changed;
    
    GLKMatrix4 shadowRenderMatrix;
    GLKMatrix4 bodyRenderMatrix;
    GLKMatrix4 headRenderMatrix;
    
    double _lastHeadAnimTime;
    
    
    vector<int>         _connectorFrames;
    vector<GLKMatrix4>  _connectorMatrices;
    bool                _connectorsChanged;
    
    
    float   _bridgeScale;
    double  _bridgeStartAnim;
    bool    _bridgeAnimDirection;
    bool    _animRunned;
    
public:
    
    
    int FrameForConnectorLocation(MAXUNITOBJECT_CONNECTOR connector) const;
    void AddConnector(MAXUNITOBJECT_CONNECTOR connector);
    void RemoveConnectors();
    void UpdateConnectors();
    
    GLKVector4 playerColor;
    
    void SetBodyOffset (int offset);
    
    int bodyOffset;
    int headOffset;
    int headFireOffset;
    
    MAXUnitObjectDelegate* _delegate_w;
    
    Texture** _playerPalette_w;
    int _playerId;
    
    bool _needAirOffset;
    float _airOffsetMult;
    bool _needShipOffset;
    bool _needShadow;
    bool _highLevel;
    
    virtual GLKVector2 GetDeltaPosition() const;
    
    bool GetFireing() const {return fireing;};
    int GetBodyIndex() const {return bodyIndex - bodyOffset;};
    int GetHeadIndex() const {return headIndex;};
    int GetPureHeadIndex() const {return pureheadIndex;};
    
	void LiftBridge();
	void DropBridge();


    bool IsSingleFire() const;
    bool IsHasBody() const;
    bool CanFire() const;
    FIRE_TYPE FireType() const;
    
    
    MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXObjectConfig* config);
    ~MAXUnitObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual void Draw(Shader *shader);
    virtual void DrawLow(Shader *shader);
    virtual void Frame(double time);
    virtual void LastUpdate(bool low);
    
    GLKMatrix4 CalculateLowRenderMatrix();
    GLKMatrix4 CalculateShadowRenderMatrix();
    GLKMatrix4 CalculateBodyRenderMatrix();
    GLKMatrix4 CalculateHeadRenderMatrix();
    GLKVector2 CalculateAirOffset() const;
    GLKVector2 CalculateShipOffset() const;
    GLKMatrix4 CalculateUnitCenterMatrix() const;
    
    void SetBodyDirection(int state);
    void SetHeadDirection(int state);
    void SetIsFireing(bool fire, bool ligthFrame);
    
    
};

#endif /* defined(__MAX__MAXUnitObject__) */
