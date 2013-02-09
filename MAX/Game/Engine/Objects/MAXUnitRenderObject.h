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

using namespace std;

class Material;
class EngineMesh;

class MAXUnitRenderObject : public RenderObject {
    //not own
    shared_ptr<EngineMesh> _mesh;
public:
    
    MAXUnitRenderObject(shared_ptr<EngineMesh> mesh);
    ~MAXUnitRenderObject();


    virtual void Render(int lod, Material *material);
    void RenderShadow(int lod, Material *material);
    
    void Bind();
    void UnBind();
};

#endif /* defined(__MAX__MAXUnitRenderObject__) */
