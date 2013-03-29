//
//  MAXREsourceMapMaterial.h
//  MAX
//
//  Created by Anton Katekov on 29.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXREsourceMapMaterial__
#define __MAX__MAXREsourceMapMaterial__

#include <iostream>
#include "Material.h"

class Texture;

class MAXREsourceMapMaterial : public Material {
    
public:
    
    Texture *_texture_w;
    
    MAXREsourceMapMaterial();
    ~MAXREsourceMapMaterial();
    
    virtual void ApplyLod(int lod, Shader *shader);
    virtual void SetFrame(int frame);
    virtual void StopBackgroundLoading();
    
};

#endif /* defined(__MAX__MAXREsourceMapMaterial__) */
