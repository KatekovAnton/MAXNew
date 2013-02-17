//
//  SceneSystem.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/5/12.
//
//

#include "SceneSystem.h"
#include "PivotObject.h"
#include "MAXSceneGraph.h"
#include "MAXMapObject.h"

using namespace std;

SceneSystem::SceneSystem(MAXMapObject* map)
{
    
    _map_w = map;
    _objects = new UContainer<PivotObject>(100);
    _visibleObjects = new UContainer<PivotObject>(100);
    _shadowObjects = new UContainer<PivotObject>(100);
    
    _movedObjects_w = new USimpleContainer<PivotObject*>(100);
    
    _sceneGraph = new MAXSceneGraph(this);
}

SceneSystem::~SceneSystem()
{
    delete _objects;
    delete _visibleObjects;
    delete _shadowObjects;
    delete _movedObjects_w;
    delete _sceneGraph;
}

void SceneSystem::CalculateVisbleObject()
{
    
    //_sceneGraph.calculateVisibleObjects(CameraControllers.CameraManager.Camera.cameraFrustum, _visibleObjects);
    
    _visibleObjects->AddObjects(_objects);
}

void SceneSystem::BeginFrame()
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        shared_ptr<PivotObject> obj = _objects->objectAtIndex(i);
        obj->BeginFrame();
    }
}

void SceneSystem::Frame(double time)
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        _objects->objectAtIndex(i)->Frame(time);
    }
}

void SceneSystem::EndFrame()
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        _objects->objectAtIndex(i)->EndFrame();
    }
}

void SceneSystem::UpdateScene()
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        PivotObject* object = _objects->objectAtIndex(i).get();
        object->Update();
    }
    //_sceneGraph.NewFrame();
    _visibleObjects->clear();
    _shadowObjects->clear();
}

void SceneSystem::AfterUpdate()
{
    _movedObjects_w->clear();
    float resultX;
    float resultY;
    BoundingBox result;
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        PivotObject* object = _objects->objectAtIndex(i).get();
        object->AfterUpdate();
        if (object->moved)
        {
            _movedObjects_w->addObject(object);
            
            //update bs
            resultX = object->GetTransformMatrix().m30 - 1 + _map_w->mapW/2;
            resultY = -1 * (object->GetTransformMatrix().m31 - _map_w->mapH/2) - 1;
            
            result.min.x = resultX + 0.1;
            result.min.y = resultY + 0.1;
            result.max.x = resultX - 0.1 + object->_size.x;
            result.max.y = resultY - 0.1 + object->_size.y;
            object->_boundingShape = result;
            
            object->moved = false;
        }
    }
}

void SceneSystem::LastUpdate()
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        PivotObject* object = _objects->objectAtIndex(i).get();
        object->LastUpdate();
    }
}

void SceneSystem::Clear()
{
    _visibleObjects->clear();
    _shadowObjects->clear();
    _objects->clear();
}

void SceneSystem::AddObject(const shared_ptr<PivotObject>& newObject, bool needUpdate)
{
    
    //newObject->_objectBehaviourModel->Enable();
    _objects->addObject(newObject);
    newObject->AfterUpdate();
    //_sceneGraph->AddObject((MAXObject*)newObject.get());
}

shared_ptr<PivotObject> SceneSystem::GetObject(unsigned int objId)
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        if (_objects->objectAtIndex(i)->GetObjectId() == objId)
        {
            return _objects->objectAtIndex(i);
        }
    }
    return nullptr;
}

void SceneSystem::DeleteObjects(UContainer<PivotObject> *objects)
{
    
}

void SceneSystem::RemoveObject(const shared_ptr<PivotObject>& object)
{
    _objects->removeObject(object);
    //_sceneGraph.RemoveObject(deletingobjects);
    //deletingobjects.behaviourmodel.Disable();
    if (_objects->GetCount() == 0)
        IdGenerator::DefaultGenerator()->Reset();
}

