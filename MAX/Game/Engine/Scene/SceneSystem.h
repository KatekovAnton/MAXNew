//
//  SceneSystem.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/5/12.
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
    
    USimpleContainer<PivotObject*>  *_visibleObjects;
    USimpleContainer<PivotObject*>  *_objects;
    
    
    USimpleContainer<PivotObject*>         *_movedObjects_w;
    
    MAXMapObject                    *_map_w;
    
    MAXSceneGraph                   *_sceneGraph;
    
    void CalculateBBForObject(PivotObject* object);
    
public:
    
    SceneSystem(MAXMapObject* map);
    ~SceneSystem();
    
    USimpleContainer<PivotObject*>* GetVisibleObjects() const { return _visibleObjects; };
    USimpleContainer<PivotObject*>* GetObjects() const { return _objects; };
    
    MAXMapObject *GetMap() const {return _map_w; };
    void Clear();
    
    PivotObject* GetObject(unsigned int objId);
    void AddObject(PivotObject* newObject, bool needUpdate);
    //not imlemented
    void DeleteObjects(UContainer<PivotObject> *objects);
    void RemoveObject(PivotObject* object);
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
