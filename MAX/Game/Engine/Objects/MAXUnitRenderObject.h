//
//  MAXUnitRenderObject.h
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXUnitRenderObject__
#define __MAX__MAXUnitRenderObject__

#include <iostream>
#include "RenderObject.h"

class MAXUnitMaterial;
class Material;
class EngineMesh;

class MAXUnitRenderObject : public RenderObject {
    //not own
    EngineMesh* _mesh;
public:
    MAXUnitMaterial* material;
    
    MAXUnitRenderObject();
    ~MAXUnitRenderObject();


    virtual void Render(int lod, Material *material);
};

#endif /* defined(__MAX__MAXUnitRenderObject__) */
