//
//  SceneGraphNode.h
//  MAX
//
//  Created by  Developer on 17.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXSceneGraphNode__
#define __MAX__MAXSceneGraphNode__

#include <iostream>
#include "miniPrefix.h"

class PivotObject;

class MAXSceneGraphNode {
    
public:
    
    MAXSceneGraphNode                       *_parentNode_w;
    MAXSceneGraphNode                       *_childNodes[4];
    BoundingBox                             _boundingRect;
    int                                     _nestingLevel;
    USimpleContainer<PivotObject*>          *_entities_w;
    
    
    MAXSceneGraphNode(MAXSceneGraphNode *parentNode, BoundingBox bb, int level);
    ~MAXSceneGraphNode();
};

#endif /* defined(__MAX__SceneGraphNode__) */
