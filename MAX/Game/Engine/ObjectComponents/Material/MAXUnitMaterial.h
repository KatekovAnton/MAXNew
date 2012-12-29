//
//  MAXUnitMaterial.h
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXUnitMaterial__
#define __MAX__MAXUnitMaterial__

#include <iostream>
#include "Material.h"
#include "Geometry.h"

class Texture;

class MAXUnitMaterialFrame {
public:
    GLKVector2 center;
    GLKVector2 size;
};

class MAXUnitMaterial : public Material {

public:

    int frameCount;
    
    int index;
    double time;
    
    
    MAXUnitMaterialFrame* frames;
    Texture** textures;
    
    MAXUnitMaterial(int frameCount);
    ~MAXUnitMaterial();
    
    void DoFrame(double elapsedTime);
    
    virtual void ApplyLod(int lod, Shader *shader);
    virtual void SetFrame(int frame);
    virtual void StopBackgroundLoading();
};

#endif /* defined(__MAX__MAXUnitMaterial__) */
