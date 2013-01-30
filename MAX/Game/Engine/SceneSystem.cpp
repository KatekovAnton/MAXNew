//
//  SceneSystem.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/5/12.
//
//

#include "SceneSystem.h"
#include "PivotObject.h"

using namespace std;

SceneSystem::SceneSystem() {
    _objects = new UContainer<PivotObject>(100);
    _visibleObjects = new UContainer<PivotObject>(100);
    _shadowObjects = new UContainer<PivotObject>(100);
    
    _userInterface = new UserInterface;
    
   
}

SceneSystem::~SceneSystem() {
    delete _objects;
    delete _visibleObjects;
    delete _shadowObjects;
    delete _userInterface;
}

void SceneSystem::UpdateScene() {
    for (int i = 0; i < _objects->GetCount(); i++) {
        _objects->objectAtIndex(i)->Update();
    }
    //_sceneGraph.NewFrame();
    _visibleObjects->clear();
    _shadowObjects->clear();
}

void SceneSystem::AfterUpdate() {
    for (int i = 0; i < _objects->GetCount(); i++) {
        _objects->objectAtIndex(i)->AfterUpdate();
    }
}

void SceneSystem::CalculateVisbleObject() {
    //_sceneGraph.calculateVisibleObjects(CameraControllers.CameraManager.Camera.cameraFrustum, _visibleObjects);
    //_sceneGraph.calculateShadowVisibleObjects(GameEngine.Instance.GraphicPipeleine.frustumForShadow, _shadowObjects);
    
    _visibleObjects->AddObjects(_objects);
}

UserInterface * SceneSystem::GetInterfaceManager() {
    return _userInterface;
}

void SceneSystem::BeginFrame() {
    for (int i = 0; i < _objects->GetCount(); i++) {
        shared_ptr<PivotObject> obj = _objects->objectAtIndex(i);
        obj->BeginFrame();
    }
}

void SceneSystem::Frame(double time) {
    for (int i = 0; i < _objects->GetCount(); i++) {
        _objects->objectAtIndex(i)->Frame(time);
    }
}

void SceneSystem::EndFrame() {
    for (int i = 0; i < _objects->GetCount(); i++) {
        _objects->objectAtIndex(i)->EndFrame();
    }
}

void SceneSystem::Clear() {
    _visibleObjects->clear();
    _shadowObjects->clear();
    _objects->clear();
    
    IdGenerator::DefaultGenerator()->Reset();
}

void SceneSystem::AddObject(const shared_ptr<PivotObject>& newObject, bool needUpdate) {
    
    //newObject->_objectBehaviourModel->Enable();
    _objects->addObject(newObject);
    newObject->AfterUpdate();
    //_sceneGraph.AddObject(newObject);
}

shared_ptr<PivotObject> SceneSystem::GetObject(unsigned int objId) {
    for (int i = 0; i < _objects->GetCount(); i++) {
        if (_objects->objectAtIndex(i)->GetObjectId() == objId) {
            return _objects->objectAtIndex(i);
        }
    }
    return nullptr;
}

void SceneSystem::DeleteObjects(UContainer<PivotObject> *objects) {
    
}

void SceneSystem::RemoveObject(const shared_ptr<PivotObject>& object) {
    _objects->removeObject(object);
    //_sceneGraph.RemoveObject(deletingobjects);
    //deletingobjects.behaviourmodel.Disable();
    if (_objects->GetCount() == 0)
        IdGenerator::DefaultGenerator()->Reset();
}

