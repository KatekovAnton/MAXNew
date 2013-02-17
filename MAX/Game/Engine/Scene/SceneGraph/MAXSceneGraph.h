//
//  MAXSceneGraph.h
//  MAX
//
//  Created by  Developer on 17.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXSceneGraph__
#define __MAX__MAXSceneGraph__

#include <iostream>
#include "miniPrefix.h"

class SceneSystem;
class MAXObject;
class MAXSceneGraphNode;

class MAXSceneGraph {
    
    int _recalculatedCount;
    SceneSystem* _scene_w;
    
    
    float _mapSize;
    
    //graph
    MAXSceneGraphNode           *_baseNode;
    stack<MAXSceneGraphNode*>   _subtreeStack;
    
    int _maxNestingLevel = 3;
    stack<MAXSceneGraphNode*>   _nodeStack;
    map<int, MAXSceneGraphNode*> _objectNodeMap;
    MAXSceneGraphNode** _leafs;
    int _leafMassiveSize;
    float _leafSize;
    float _updateTime;
    int _visibleObjectCount;
    int _nodeTestCount;
    int _entityTestCount;
    long _queryTime;
    int _entityRecalculateCount;
    //Stopwatch _timer;
    GLKVector2 leafSize;
    
    MAXSceneGraphNode* GetLeaf(MAXObject* entity);
    void SplitNode(MAXSceneGraphNode* parent);
    int GetLeafArrayIndex(int x, int y) const;
    void Build();
public:
    
    MAXSceneGraph(SceneSystem* scene);
    ~MAXSceneGraph();
    
    void AddObject(MAXObject *newObject);
    
    void RemoveObject(MAXObject *object);
    
    void NewFrame();
    
    void CalculateVisibleObjects(BoundingBox viewField, USimpleContainer<MAXObject*> *container);
    
    void Clear();
    
    int GetRecalulcalatedObjectsCount() const {return _recalculatedCount;};
    
    void Query(BoundingBox viewField, USimpleContainer<MAXObject*> *container);
    
};

#endif /* defined(__MAX__MAXSceneGraph__) */
