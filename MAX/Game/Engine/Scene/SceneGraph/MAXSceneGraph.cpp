//
//  MAXSceneGraph.cpp
//  MAX
//
//  Created by  Developer on 17.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXSceneGraph.h"
#include "SceneSystem.h"
#include "MAXMapObject.h"
#include "MAXSceneGraphNode.h"
#include "PivotObject.h"

unsigned int nextPOT(unsigned int x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

MAXSceneGraph::MAXSceneGraph(SceneSystem* scene)
{
    _scene_w = scene;
    _mapSize = nextPOT(MAX(scene->GetMap()->mapW, scene->GetMap()->mapH));
    _maxNestingLevel = log2(_mapSize);
    _maxNestingLevel -= 4;
    if (_maxNestingLevel<1) {
        _maxNestingLevel = 1;
    }
    _leafs = NULL;
    Build();
}


MAXSceneGraph::~MAXSceneGraph()
{
    free(_leafs);
    delete _baseNode;
}

void MAXSceneGraph::Build()
{
    BoundingBox rootBbox =  BoundingBoxMake(0, 0, _mapSize, _mapSize);
    
    //инициализируем дерево
    _baseNode = new MAXSceneGraphNode(NULL, rootBbox, 0);
    
    while (!_nodeStack.empty())
        _nodeStack.pop();
    
    _nodeStack.push(_baseNode);
    
    
    while (!_nodeStack.empty())
    {
        MAXSceneGraphNode *parent = _nodeStack.top();
        _nodeStack.pop();
        if (parent->_nestingLevel < _maxNestingLevel)
        {
            SplitNode(parent);
            if (parent->_nestingLevel + 1 < _maxNestingLevel)
                for (int i = 0; i < 4; i++) {
                    MAXSceneGraphNode* child = parent->_childNodes[i];
                    _nodeStack.push(child);
                }
                   
        }
    }
    MAXSceneGraphNode* node = _leafs[GetLeafArrayIndex(0, 0)];
    leafSize = node->_boundingRect.GetSize();
}

int MAXSceneGraph::GetLeafArrayIndex(int x, int y) const
{
    return y * _leafMassiveSize + x;
}

void MAXSceneGraph::SplitNode(MAXSceneGraphNode* parent)
{
    GLKVector2 min = parent->_boundingRect.min;
    GLKVector2 max = parent->_boundingRect.max;
    
    //получаем координаты центра нода
    GLKVector2 c = GLKVector2Make((min.x + max.x)/2.0, (min.y + max.y) / 2.0);
    
    //получаем дочерние ноды
    BoundingBox tempBbox;
    int newlwvwl = parent->_nestingLevel + 1;
    
    
    //левый верхний передний
    tempBbox.min = GLKVector2Make(min.x, min.y);
    tempBbox.max = GLKVector2Make(c.x, c.y);
    parent->_childNodes[0] = new MAXSceneGraphNode(parent, tempBbox, newlwvwl);
    
    //правый верхний передний
    tempBbox.min = GLKVector2Make(c.x, min.y);
    tempBbox.max = GLKVector2Make(max.x, c.y);
    parent->_childNodes[1] = new MAXSceneGraphNode(parent, tempBbox, newlwvwl);
    
    //левый верхний задний
    tempBbox.min = GLKVector2Make(min.x, c.y);
    tempBbox.max = GLKVector2Make(c.x, max.y);
    parent->_childNodes[2] = new MAXSceneGraphNode(parent, tempBbox, newlwvwl);
    
    //правый верхний задний
    tempBbox.min = GLKVector2Make(c.x, c.y);
    tempBbox.max = GLKVector2Make(max.x, max.y);
    parent->_childNodes[3] = new MAXSceneGraphNode(parent, tempBbox, newlwvwl);
    
    
    
    //заносим новые узлы в массив листьев, если уровень вложенности достиг максимального
    if (newlwvwl >= _maxNestingLevel)
    {
        if (_leafs == NULL)
        {
            _leafSize = parent->_childNodes[0]->_boundingRect.max.x - parent->_childNodes[0]->_boundingRect.min.x;
            _leafMassiveSize = ((_baseNode->_boundingRect.max.x - _baseNode->_boundingRect.min.x) / _leafSize);
            _leafs = (MAXSceneGraphNode**)malloc(_leafMassiveSize * _leafMassiveSize * sizeof(MAXSceneGraphNode*));
            memset(_leafs, 0, _leafMassiveSize * _leafMassiveSize * sizeof(MAXSceneGraphNode*));
        }
        for (int i = 0; i < 4; i++)
        {
            //заносим листы в массив
            GLKVector2 center = parent->_childNodes[i]->_boundingRect.GetCenter();
            int x = fabsf((_baseNode->_boundingRect.min.x - center.x) / _leafSize);
            int y = fabsf((_baseNode->_boundingRect.min.y - center.y) / _leafSize);
            _leafs[GetLeafArrayIndex(x, y)] = parent->_childNodes[i];
        }
    }
}

MAXSceneGraphNode* MAXSceneGraph::GetLeaf(PivotObject* entity)
{
    BoundingBox rootBbox = _baseNode->_boundingRect;
    
    
    //получаем индексы листа, в котором расположена точка Min bbox'а объекта
    int minX = (int)((entity->_boundingShape.min.x - rootBbox.min.x) / leafSize.x);
    if (minX < 0)
        minX = 0;
    
    if (minX > _leafMassiveSize - 1)
        minX = _leafMassiveSize - 1;
    
    int minZ = (int)((entity->_boundingShape.min.y - rootBbox.min.y) / leafSize.y);
    if (minZ < 0)
        minZ = 0;
    
    if (minZ > _leafMassiveSize - 1)
        minZ = _leafMassiveSize - 1;
    
    
    return _leafs[GetLeafArrayIndex(minX, minZ)];
}

void MAXSceneGraph::RegistrateEntity(PivotObject *entity, MAXSceneGraphNode *node)
{
    node->_entities_w->addObject(entity);
    if (_objectNodeMap.count(entity) == 1)
        _objectNodeMap[entity] = node;
    else
        _objectNodeMap.insert(pair<PivotObject*, MAXSceneGraphNode*>(entity, node));
}

void MAXSceneGraph::AddObject(PivotObject *entity)
{
    MAXSceneGraphNode* leaf = GetLeaf(entity);
    while (!_nodeStack.empty())
        _nodeStack.pop();
    
    _nodeStack.push(leaf);
    
    while (!_nodeStack.empty())
    {
        MAXSceneGraphNode* node = _nodeStack.top();
        _nodeStack.pop();
        
        ContainmentType containmentType = GetContainmentType(node->_boundingRect, entity->_boundingShape);
        switch (containmentType)
        {
            case ContainmentType_Contains: //если объект полностью помещается в узел, то регистрируем его в этом узле
            {
                RegistrateEntity(entity, node);
            } break;
            case ContainmentType_Disjoint://если объект не пересекается с узлом вообще, то значит объект вышел за пределы дерева
            {
                //поэтому регистрируем его в листе, который вычислили ранее
                RegistrateEntity(entity, leaf);
            } break;
            case ContainmentType_Intersects://если объект пересекается с узлом, то нужно проверить является ли узел корневым
            {
                if (node->_parentNode_w == NULL)
                    //если да, то объект лежит на границе дерева, поэтому регистрируем его в ЛИСТЕ
                    RegistrateEntity(entity, leaf);
                else
                    //если нет, то передаем объект в родительский узел
                    _nodeStack.push(node->_parentNode_w);
            } break;
        }
    }
}

void MAXSceneGraph::RemoveObject(PivotObject* wo)
{
    if (!_objectNodeMap.count(wo))
        return;
    
    MAXSceneGraphNode *node = _objectNodeMap[wo];
    node->_entities_w->removeObject(wo);
    _objectNodeMap.erase(wo);
}

void MAXSceneGraph::Clear()
{
    for (int i = 0; i < _scene_w->GetObjects()->GetCount(); i++)
    {
        PivotObject* obj = _scene_w->GetObjects()->objectAtIndex(i);
        RemoveObject(obj);
    }
}

void MAXSceneGraph::CalculateVisibleObjects(BoundingBox viewField, USimpleContainer<PivotObject*> *container)
{
    container->clear();
    Query(viewField, container);
}

void MAXSceneGraph::GetSubtree(MAXSceneGraphNode *node, USimpleContainer<PivotObject*> *visibleEntities)
{
    while (!_subtreeStack.empty())
        _subtreeStack.pop();
    _subtreeStack.push(node);
    
    while (!_subtreeStack.empty())
    {
        MAXSceneGraphNode *n = _subtreeStack.top();
        _subtreeStack.pop();
        
        for (int i = 0; i < n->_entities_w->GetCount(); i++)
        {
            PivotObject* obj = n->_entities_w->objectAtIndex(i);
            obj->SetIsOnScreen(true);
        }
        
        visibleEntities->AddObjects(n->_entities_w);
        if (n->_nestingLevel!=_maxNestingLevel)
            for (int i = 0; i < 4; i++)
            {
                _subtreeStack.push(n->_childNodes[i]);
            }
    }
}

void MAXSceneGraph::Query(BoundingBox frustum, USimpleContainer<PivotObject*> *visibleEntities)
{
    _visibleObjectCount = 0;
    _nodeTestCount = 0;
    _entityTestCount = 0;
    
    ContainmentType containmentType = ContainmentType_Disjoint;
    
    while (!_nodeStack.empty())
        _nodeStack.pop();
    _nodeStack.push(_baseNode);
    
    while (!_nodeStack.empty())
    {
        MAXSceneGraphNode *node = _nodeStack.top();
        _nodeStack.pop();
        _nodeTestCount++;
        containmentType = GetContainmentType(frustum, node->_boundingRect);
        
        switch (containmentType)
        {
                //если узел полностью входит в пирамиду,
                //то заносим все поддерево в список видимых сущностей
            case ContainmentType_Contains:
            {
                GetSubtree(node, visibleEntities);
            } break;
                
                //case ContainmentType.Disjoint:
                // ничего не делаем
                //    break;
                
                //если узел пересекается с пирамидой, то проверяе видимость всех его объектов
                //а вложенные узлы добавляем в стэк для дальнейшей проверки
            case ContainmentType_Intersects:
            {
                ContainmentType entContType = ContainmentType_Disjoint;
                for (int i = 0; i < node->_entities_w->GetCount(); i++)
                {
                    PivotObject *wo = node->_entities_w->objectAtIndex(i);
                    _entityTestCount++;
                    entContType = GetContainmentType(frustum, wo->_boundingShape);
                    if (entContType != ContainmentType_Disjoint)
                    {
                        visibleEntities->addObject(wo);
                        wo->SetIsOnScreen(true);
                    }
                }
                if(node->_nestingLevel!=_maxNestingLevel)
                    for (int i = 0; i < 4; i++)
                    {
                        _nodeStack.push(node->_childNodes[i]);
                    }
            }break;
            default: break;
        }
    }
}

void MAXSceneGraph::Update(USimpleContainer<PivotObject*>* objects)
{
    _entityRecalculateCount = 0;
    for (int i = 0; i < objects->GetCount(); i++)
    {
        PivotObject* obj = objects->objectAtIndex(i);
  
        _entityRecalculateCount++;
        RemoveObject(obj);
        AddObject(obj);
        
    }
}
