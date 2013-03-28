//
//  SceneSystem.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/5/12.
//
//

#include "SceneSystem.h"
#include "PivotObject.h"
#include "MAXSceneGraph.h"
#include "MAXMapObject.h"
#include "MAXEngine.h"

using namespace std;

SceneSystem::SceneSystem(MAXMapObject* map)
{
    
    _map_w = map;
    _objects = new USimpleContainer<PivotObject*>(100);
    _visibleObjects = new USimpleContainer<PivotObject*>(100);
    
    _movedObjects_w = new USimpleContainer<PivotObject*>(100);
    
    _sceneGraph = new MAXSceneGraph(this);
}

SceneSystem::~SceneSystem()
{
    delete _objects;
    delete _visibleObjects;
    delete _movedObjects_w;
    delete _sceneGraph;
}

void SceneSystem::CalculateVisbleObject()
{
    BoundingBox viewField;
    CCRect rect = engine->ScreenToWorldRect();
    viewField.min.x = rect.origin.x / 64.0;
    viewField.min.y = rect.origin.y / 64.0;
    
    viewField.max.x = rect.origin.x / 64.0 + rect.size.width / 64.0;
    viewField.max.y = rect.origin.y / 64.0 + rect.size.height / 64.0;
    
    _sceneGraph->CalculateVisibleObjects(viewField, _visibleObjects);
    
    //_visibleObjects->AddObjects(_objects);
}

void SceneSystem::BeginFrame()
{
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        PivotObject* obj = _objects->objectAtIndex(i);
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
        PivotObject* object = _objects->objectAtIndex(i);
        object->Update();
    }
    //_sceneGraph.NewFrame();
    _visibleObjects->clear();
}

void SceneSystem::AfterUpdate()
{
    _movedObjects_w->clear();
    for (int i = 0; i < _objects->GetCount(); i++)
    {
        PivotObject* object = _objects->objectAtIndex(i);
        object->AfterUpdate();
        if (object->moved)
        {
            _movedObjects_w->addObject(object);
            CalculateBBForObject(object);
            object->moved = false;
        }
    }
    _sceneGraph->Update(_movedObjects_w);
}

void SceneSystem::LastUpdate(bool low)
{
    for (int i = 0; i < _visibleObjects->GetCount(); i++)
    {
        PivotObject* object = _visibleObjects->objectAtIndex(i);
        object->LastUpdate(low);
    }
}

void SceneSystem::Clear()
{
    _visibleObjects->clear();
    _objects->clear();
}

void SceneSystem::CalculateBBForObject(PivotObject* object)
{
    float resultX;
    float resultY;
    BoundingBox result;
    //update bs
    resultX = object->GetTransformMatrix().m30 - 1 + _map_w->mapW/2;
    resultY = -1 * (object->GetTransformMatrix().m31 - _map_w->mapH/2) - 1;
    
    result.min.x = resultX + 0.1 - object->_bbsize.x + 1;
    result.min.y = resultY + 0.1 - object->_bbsize.y + 1;
    result.max.x = resultX - 0.1 + object->_bbsize.x;
    result.max.y = resultY - 0.1 + object->_bbsize.y;
    object->_boundingShape = result;
}

void SceneSystem::AddObject(PivotObject* newObject, bool needUpdate)
{
    //newObject->_objectBehaviourModel->Enable();
    _objects->addObject(newObject);
    newObject->AfterUpdate();
    
    CalculateBBForObject(newObject);
    
    _sceneGraph->AddObject(newObject);
}

void SceneSystem::GetAllObjectsInArea(BoundingBox bb, USimpleContainer<PivotObject*> *buffer)
{
    _sceneGraph->Query(bb, buffer);
}

PivotObject* SceneSystem::GetObject(unsigned int objId)
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

void SceneSystem::RemoveObject(PivotObject* object)
{
    _objects->removeObject(object);
    _sceneGraph->RemoveObject(object);
    //deletingobjects.behaviourmodel.Disable();
    if (_objects->GetCount() == 0)
        IdGenerator::DefaultGenerator()->Reset();
}

