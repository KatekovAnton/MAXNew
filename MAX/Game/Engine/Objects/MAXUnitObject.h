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
#include "MAXUnitConfig.h"
#include "MAXStatusRendererUnitDelegate.h"

class MAXUnitRenderObject;
class MAXUnitMaterial;
class Texture;

using namespace cocos2d;

enum
{
    MAXUnitObjectTypeSingle,
    MAXUnitObjectTypeTurreted
} typedef MAXUnitObjectType;



class MAXUnitObject : public MAXObject {
    
    
    double _random;
    
    MAXUnitRenderObject        *_renderAspect;//own
    MAXUnitMaterial            *_material;//own
    
    int bodyIndex;
    int headIndex;
    
    int purebodyIndex;
    int pureheadIndex;
    
    bool fireing;
    
    bool changed;
    
    GLKMatrix4 shadowRenderMatrix;
    GLKMatrix4 bodyRenderMatrix;
    GLKMatrix4 headRenderMatrix;
    
    int bodyOffset = 0;
    int headOffset = 8;
    int headFireOffset = 16;
    
    double _lastHeadAnimTime;
    
public:
    
    MAXStatusRendererUnitDelegate* _statusDelegate_w;
    
    Texture* _playerPalette_w;
    int _playerId;
    MAXUnitConfig* params_w;
    
    bool _needAirOffset;
    float _airOffsetMult;
    bool _needShipOffset;
    
    bool GetFireing() const {return fireing;};
    int GetBodyIndex() const {return bodyIndex;};
    int GetHeadIndex() const {return headIndex;};
    int GetPureHeadIndex() const {return pureheadIndex;};
    


    bool IsSingleFire() const;
    bool IsHasBody() const;
    bool CanFire() const;
    
    float deltax;
    float deltay;
    
    MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXUnitConfig* config);
    ~MAXUnitObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual void Draw(Shader *shader);
    virtual void Frame(double time);
    virtual void LastUpdate();
    
    virtual compareFunc GetCompareFunc();
    
    GLKMatrix4 CalculateShadowRenderMatrix();
    GLKMatrix4 CalculateBodyRenderMatrix();
    GLKMatrix4 CalculateHeadRenderMatrix();
    GLKVector2 CalculateAirOffset();
    GLKVector2 CalculateShipOffset();
    
    void SetBodyDirection(int state);
    void SetHeadDirection(int state);
    void SetIsFireing(bool fire, bool ligthFrame);
    
    
};

#endif /* defined(__MAX__MAXUnitObject__) */
