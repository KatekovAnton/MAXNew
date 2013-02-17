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
class PivotObject;
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
    map<PivotObject*, MAXSceneGraphNode*> _objectNodeMap;
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
    
    
    MAXSceneGraphNode* GetLeaf(PivotObject* entity);
    void SplitNode(MAXSceneGraphNode* parent);
    int GetLeafArrayIndex(int x, int y) const;
    void RegistrateEntity(PivotObject *entity, MAXSceneGraphNode *node);
    void Build();
    void GetSubtree(MAXSceneGraphNode *node, USimpleContainer<PivotObject*> *visibleEntities);
    
public:
    
    int GetRecalulcalatedObjectsCount() const {return _recalculatedCount;};
    
    MAXSceneGraph(SceneSystem* scene);
    ~MAXSceneGraph();
    
    void AddObject(PivotObject *newObject);
    void RemoveObject(PivotObject *object);
    void Clear();

    
    void CalculateVisibleObjects(BoundingBox viewField, USimpleContainer<PivotObject*> *container);
    void Query(BoundingBox viewField, USimpleContainer<PivotObject*> *container);
    
    
    void Update(USimpleContainer<PivotObject*> *container);
    
    
    
};

#endif /* defined(__MAX__MAXSceneGraph__) */
