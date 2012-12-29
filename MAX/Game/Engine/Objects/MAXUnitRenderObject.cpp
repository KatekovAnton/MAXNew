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

using namespace std;

MAXUnitRenderObject::MAXUnitRenderObject(shared_ptr<EngineMesh> mesh)
{
    _mesh = mesh;
}

MAXUnitRenderObject::~MAXUnitRenderObject()
{
    //dont delete material!
}

void MAXUnitRenderObject::Bind()
{
    _mesh->Bind();
}

void MAXUnitRenderObject::UnBind()
{
    _mesh->Unbind();
}

void MAXUnitRenderObject::Render(int lod, Material *material)
{
    material->ApplyLod(lod, engine->GetShader());    
    _mesh->Render();
}