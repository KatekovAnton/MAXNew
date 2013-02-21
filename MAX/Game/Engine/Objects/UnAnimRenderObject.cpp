//
//  UnAnimRenderObject.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/3/12.
//
//

#include "UnAnimRenderObject.h"
#include "EngineMesh.h"
#include "Material.h"
#include "MAXEngine.h"

UnAnimRenderObject::UnAnimRenderObject(shared_ptr<EngineMesh> mesh):RenderObject() {
    _mesh = mesh;
}

void UnAnimRenderObject::Render(int lod, Material *material) {
    material->ApplyLod(lod, engine->GetShader());
    _mesh->Bind();
    _mesh->Render();
    _mesh->Unbind();
}

UnAnimRenderObject::~UnAnimRenderObject() {

    
}