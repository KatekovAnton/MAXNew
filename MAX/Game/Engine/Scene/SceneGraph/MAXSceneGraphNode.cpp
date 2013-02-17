//
//  SceneGraphNode.cpp
//  MAX
//
//  Created by  Developer on 17.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXSceneGraphNode.h"

MAXSceneGraphNode::MAXSceneGraphNode(MAXSceneGraphNode *parentNode, BoundingBox bb, int level)
{
    _boundingRect = bb;
    _parentNode_w = parentNode;
    _nestingLevel = level;
    _entities_w = new USimpleContainer<PivotObject*>(100);
    memset(_childNodes, 4*sizeof(size_t), 0);
}

MAXSceneGraphNode::~MAXSceneGraphNode()
{
    for (int i = 0; i < 4; i++)
    {
        MAXSceneGraphNode* node = _childNodes[i];
        if (node)
            delete node;
    }
    
    delete _entities_w;
}