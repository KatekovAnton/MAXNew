//
//  MapRenderObject.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MapRenderObject.h"
#include "MAXContentMap.h"
#include "EngineMesh.h"
#include "Material.h"
#include "MAXEngine.h"

MapRenderObject::MapRenderObject(shared_ptr<MAXContentMap> map):RenderObject()
{
    _mesh = EngineMesh::CreateScaledQuad(map->w, map->h);
}

MapRenderObject::~MapRenderObject()
{
    
}

void MapRenderObject::Render(int lod, Material *material) {
    material->ApplyLod(lod, engine->GetShader());
    _mesh->Bind();
    _mesh->Render();
    _mesh->Unbind();
}
