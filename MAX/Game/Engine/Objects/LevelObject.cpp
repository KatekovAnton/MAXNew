//
//  LevelObject.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "LevelObject.h"
#include "EngineMesh.h"
#include "UnAnimRenderObject.h"
#include "TextureMaterial.h"
#include "Texture.h"

LevelObject::LevelObject(RenderObject *renderObject, Material *material):PivotObject() {
    _renderAspect = renderObject;
    _material = material;
}

LevelObject * LevelObject::CreateUnitQuad() {
    shared_ptr<EngineMesh> mesh = EngineMesh::CreateUnitQuad();
    Material *material = nullptr;
    
    RenderObject *renderObject = new UnAnimRenderObject(mesh);
    
    return new LevelObject(renderObject, material);
}

RenderObject * LevelObject::GetRenderAspect()
{
    return _renderAspect;
}

void LevelObject::Frame(double time)
{
    //_objectBehaviourModel->Frame(time);
   // _renderAspect->SetPosition(_cu);
}

Material * LevelObject::GetMaterial()
{
    return _material;
}

LevelObject::~LevelObject() {
    delete _renderAspect;
    delete _material;
}
