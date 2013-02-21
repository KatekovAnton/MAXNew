//
//  PivotObject.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/31/12.
//
//

#include "PivotObject.h"
#include "RenderObject.h"
#include "Material.h"
#include "Geometry.h"
#include "Shader.h"
#include "MAXEngine.h"


int compareDummy(const void * a, const void * b)
{
    return 0;
}

compareFunc PivotObject::GetCompareFunc()
{
    return &compareDummy;
}

PivotObject::PivotObject() {

    _transformMatrix = GLKMatrix4Identity;
    _isOnScreen = true;
    moved = false;
    forceHidden = false;
    _bbsize = GLKVector2Make(1, 1);
}

void PivotObject::Update() {
 //   _transformMatrix = _objectBehaviourModel->GetGlobalPosition();
//    if (moved)
//        raycastaspect.boundingShape.Update(transform);
}

void PivotObject::SetGlobalPosition(GLKMatrix4 globalPosition, void *aditionalData, PivotObject *parent, bool afterUpdate)
{
   // _objectBehaviourModel->SetGlobalPosition(globalPosition, aditionalData, parent, afterUpdate);
    moved = true;
    _transformMatrix = globalPosition;
    if (afterUpdate)
    {
       // _objectBehaviourModel->EndFrame();
        Update();
        AfterUpdate();
    }
//    else
//    {
//        _transformMatrix = _objectBehaviourModel->GetGlobalPosition();
//    }
}

void PivotObject::AfterUpdate()
{
    
}

void PivotObject::SetIsOnScreen(bool isOnScreen) {
    _isOnScreen = isOnScreen;
}

void PivotObject::BeginFrame() {
    //_objectBehaviourModel->BeginFrame();
   // SetIsOnScreen(false);
}

void PivotObject::Frame(double time) {
    //_objectBehaviourModel->Frame(time);
}

void PivotObject::EndFrame() {
   // _objectBehaviourModel->EndFrame();
   // moved = _objectBehaviourModel->moved;
   // _transformMatrix = _objectBehaviourModel->GetGlobalPosition();
}

void PivotObject::HasBeenLocatedToScene()
{
    _sceneLocationTime = engine->FullTime();
}

void PivotObject::HasBeenRemovedFromScene()
{}

void PivotObject::Draw(Shader *shader)
{
    shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, _transformMatrix.m);
    GetRenderAspect()->Render(0, GetMaterial());
}

RenderObject * PivotObject::GetRenderAspect() {
    return NULL;
}

Material * PivotObject::GetMaterial() {
    return NULL;
}

void PivotObject::SetPosition(const GLKVector3& position) {
  //  _objectBehaviourModel->SetPosition(GLKMatrix4MakeTranslationV(position));
}

PivotObject::~PivotObject() {
   // delete _objectBehaviourModel;
}

void PivotObject::LastUpdate()
{}