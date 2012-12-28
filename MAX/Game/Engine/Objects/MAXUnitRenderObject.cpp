//
//  MAXUnitRenderObject.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXUnitRenderObject.h"
#include "EngineMesh.h"
#include "Material.h"
#include "MAXEngine.h"

MAXUnitRenderObject::MAXUnitRenderObject()
{
    
}

MAXUnitRenderObject::~MAXUnitRenderObject()
{
    //dont delete material!
}

void MAXUnitRenderObject::Render(int lod, Material *material) {
    material->ApplyLod(lod, engine->GetShader());
    _mesh->Bind();
    _mesh->Render();
    _mesh->Unbind();
}