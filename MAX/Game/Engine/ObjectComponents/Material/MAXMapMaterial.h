//
//  MAXMapMaterial.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXMapMaterial__
#define __MAX__MAXMapMaterial__

#include <iostream>
#include "Material.h"

class Texture;

class MAXMapMaterial : public Material
{
public:
    Texture* mapElementsSingle;
    Texture* palettes;
    
    Texture* fullMapTexture;
};

#endif /* defined(__MAX__MAXMapMaterial__) */
