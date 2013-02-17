//
//  SceneSystem.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/5/12.
//
//

#ifndef __TerminalExtraction__SceneSystem__
#define __TerminalExtraction__SceneSystem__

#include <iostream>
#include "Utils.h"

using namespace Utils;

class PivotObject;
class MAXMapObject;
class MAXSceneGraph;

class SceneSystem {
    
    UContainer<PivotObject>         *_shadowObjects;
    UContainer<PivotObject>         *_visibleObjects;
    UContainer<PivotObject>         *_objects;
    
    
    USimpleContainer<PivotObject*>         *_movedObjects_w;
    
    MAXMapObject                    *_map_w;
    
    MAXSceneGraph                   *_sceneGraph;
    
public:
    UContainer<PivotObject>* GetAllObjects() const { return _objects; } 
    SceneSystem(MAXMapObject* map);
    ~SceneSystem();
    
    
    UContainer<PivotObject>* GetVisibleObjects() const { return _visibleObjects; };
    UContainer<PivotObject>* GetObjects() const { return _objects; };
    MAXMapObject *GetMap() const {return _map_w; };
    void Clear();
    
    shared_ptr<PivotObject> GetObject(unsigned int objId);
    void AddObject(const shared_ptr<PivotObject>& newObject, bool needUpdate);
    //not imlemented
    void DeleteObjects(UContainer<PivotObject> *objects);
    void RemoveObject(const shared_ptr<PivotObject>& object);
    void AddObject(UContainer<PivotObject> *newObjects);
    
    void UpdateScene();
    void AfterUpdate();
    void LastUpdate();
    void CalculateVisbleObject();
    
    void BeginFrame();
    void EndFrame();
    void Frame(double time);

};


#endif /* defined(__TerminalExtraction__SceneSystem__) */
