//
//  MAXEffectObject.h
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXEffectObject__
#define __MAX__MAXEffectObject__

#include <iostream>
#include "MAXObject.h"

class MAXUnitRenderObject;
class MAXUnitMaterial;
class Texture;

class MAXEffectObject : public MAXObject {
    
    MAXUnitRenderObject        *_renderAspect;//own
    MAXUnitMaterial            *_material;//not own
    GLKMatrix4 bodyRenderMatrix;
    
public:
    
    Texture* _playerPalette_w;
    int _playerId;
    
    int _frameCount;
    int _currentFrame;
    
    GLKMatrix4 CalculateBodyRenderMatrix();
    
    MAXEffectObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXObjectConfig* config);
    virtual ~MAXEffectObject();
    
    void SetEffectFrame(int frame);
    virtual void HasBeenLocatedToScene();
    virtual void Draw(Shader *shader);
    virtual void DrawLow(Shader *shader);
    virtual void Frame(double time);
    virtual void LastUpdate(bool low);
};

#endif /* defined(__MAX__MAXEffectObject__) */
