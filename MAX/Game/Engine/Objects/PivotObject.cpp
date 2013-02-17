//
//  PivotObject.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/31/12.
//
//

#include "PivotObject.h"
#include "RenderObject.h"
#include "Material.h"
#include "Geometry.h"
#include "Shader.h"
#include "MAXEngine.h"

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

GLKMatrix4 PivotObject::GetTransformMatrix() const {
    return _transformMatrix;
}

GLKVector3 PivotObject::GetPosition() const {
    return GLKMatrix4GetTranslation(_transformMatrix);
}

void PivotObject::SetPosition(const GLKVector3& position) {
  //  _objectBehaviourModel->SetPosition(GLKMatrix4MakeTranslationV(position));
}

PivotObject::~PivotObject() {
   // delete _objectBehaviourModel;
}

void PivotObject::LastUpdate()
{}