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

//
//void MAXSceneGraph::NewFrame();
//{
//    _octree.Update(_scene._objects);
//}
//
//void MAXSceneGraph::CalculateVisibleObjects(GRect2D viewField, USimpleContainer<MAXObject*> *container)
//{
//    __container.Clear();
//    _octree.Query(__viewFrustum, __container);
//}
//

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

//MAXSceneGraphNode* MAXSceneGraph::GetLeaf(MAXObject* entity)
//{
//    BoundingBox rootBbox = _baseNode->_boundingRect;
//    
//    
//    //получаем индексы листа, в котором расположена точка Min bbox'а объекта
//    int minX = (int)((entity.raycastaspect.boundingShape.aabb.XNAbb.min.x - rootBbox.min.x) / leafSize.X);
//    if (minX < 0)
//        minX = 0;
//    
//    if (minX > _leafMassiveSize - 1)
//        minX = _leafMassiveSize - 1;
//    
//    int minZ = (int)((entity.raycastaspect.boundingShape.aabb.XNAbb.min.y - rootBbox.min.y) / leafSize.Z);
//    if (minZ < 0)
//        minZ = 0;
//    
//    if (minZ > _leafMassiveSize - 1)
//        minZ = _leafMassiveSize - 1;
//    
//    
//    return _leafs[minX, minZ];
//}

//private void RegistrateEntity(PivotObject entity, MAXSceneGraphNode node)
//{
//    node.Entities.Add(entity);
//    int hashCode = entity.GetHashCode();
//    if (_objectNodeMap.ContainsKey(hashCode))
//        _objectNodeMap[hashCode] = node;
//    else
//        _objectNodeMap.Add(hashCode, node);
//}

void MAXSceneGraph::AddObject(MAXObject *entity)
{
    MAXSceneGraphNode* leaf = GetLeaf(entity);
    while (!_nodeStack.empty())
        _nodeStack.pop();
    
    _nodeStack.push(leaf);
    
    while (!_nodeStack.empty())
    {
        MAXSceneGraphNode* node = _nodeStack.top();
        _nodeStack.pop();
        
//        ContainmentType containmentType = ContainmentType.Disjoint;
//        
//        node.boundingRect.Contains(ref entity.raycastaspect.boundingShape.aabb.XNAbb, out containmentType);
//        switch (containmentType)
//        {
//            case ContainmentType.Contains: //если объект полностью помещается в узел, то регистрируем его в этом узле
//            {
//                RegistrateEntity(entity, node);
//            } break;
//            case ContainmentType.Disjoint://если объект не пересекается с узлом вообще, то значит объект вышел за пределы дерева
//            {
//                //поэтому регистрируем его в листе, который вычислили ранее
//                RegistrateEntity(entity, leaf);
//            } break;
//            case ContainmentType.Intersects://если объект пересекается с узлом, то нужно проверить является ли узел корневым
//            {
//                if (node.ParentNode == null)
//                    //если да, то объект лежит на границе дерева, поэтому регистрируем его в ЛИСТЕ
//                    RegistrateEntity(entity, leaf);
//                else
//                    //если нет, то передаем объект в родительский узел
//                    _nodeStack.Push(node.ParentNode);
//            } break;
//        }
    }
}

//private void GetSubtree(MAXSceneGraphNode node, MyContainer<PivotObject> visibleEntities)
//{
//    _subtreeStack.Clear();
//    _subtreeStack.Push(node);
//    
//    while (_subtreeStack.Count > 0)
//    {
//        MAXSceneGraphNode n = _subtreeStack.Pop();
//        PivotObject[] objects = n.Entities.ToArray();
//        foreach (PivotObject obj in objects)
//        obj.SetIsOnScreen(true);
//        
//        visibleEntities.AddRange(n.Entities.ToArray());
//        if (n._childNodes != null)
//            for (int i = 0; i < n._childNodes.Length; i++)
//            {
//                _subtreeStack.Push(n._childNodes[i]);
//            }
//    }
//}
//
//public void Query(BoundingFrustum frustum, MyContainer<PivotObject> visibleEntities)
//{
//    _visibleObjectCount = 0;
//    _nodeTestCount = 0;
//    _entityTestCount = 0;
//    _timer.Reset();
//    _timer.Start();
//    
//    ContainmentType containmentType = ContainmentType.Disjoint;
//    
//    _nodeStack.Clear();
//    _nodeStack.Push(this.baseNode);
//    
//    while (_nodeStack.Count > 0)
//    {
//        MAXSceneGraphNode node = _nodeStack.Pop();
//        _nodeTestCount++;
//        frustum.Contains(ref node.boundingRect, out containmentType);
//        
//        switch (containmentType)
//        {
//                //если узел полностью входит в пирамиду,
//                //то заносим все поддерево в список видимых сущностей
//            case ContainmentType.Contains:
//            {
//                GetSubtree(node, visibleEntities);
//            } break;
//                
//                //case ContainmentType.Disjoint:
//                // ничего не делаем
//                //    break;
//                
//                //если узел пересекается с пирамидой, то проверяе видимость всех его объектов
//                //а вложенные узлы добавляем в стэк для дальнейшей проверки
//            case ContainmentType.Intersects:
//            {
//                ContainmentType entContType = ContainmentType.Disjoint;
//                for (int i = 0; i < node.Entities.Count; i++)
//                {
//                    PivotObject wo = node.Entities[i];
//                    _entityTestCount++;
//                    entContType = ContainmentType.Disjoint;
//                    frustum.Contains(ref wo.raycastaspect.boundingShape.aabb.XNAbb, out entContType);
//                    if (entContType != ContainmentType.Disjoint)
//                    {
//                        visibleEntities.Add(wo);
//                        wo.SetIsOnScreen(true);
//                    }
//                }
//                if(node.nestingLevel!=_maxNestingLevel)
//                    for (int i = 0; i < node._childNodes.Length; i++)
//                    {
//                        _nodeStack.Push(node._childNodes[i]);
//                    }
//            }break;
//            default: break;
//        }
//    }
//    
//    _visibleObjectCount = visibleEntities.Count;
//    _timer.Stop();
//    _queryTime = _timer.ElapsedMilliseconds;
//}
//
//public bool RemoveObject(PivotObject wo)
//{
//    int hashCode = wo.GetHashCode();
//    if (!_objectNodeMap.ContainsKey(hashCode))
//    {
//        return false;
//    }
//    MAXSceneGraphNode node = _objectNodeMap[hashCode];
//    bool r1 = node.Entities.Remove(wo);
//    bool r2 = _objectNodeMap.Remove(hashCode);
//    
//    return r1 && r2;
//}
//
//public void Update(MyContainer<PivotObject> objects)
//{
//    _timer.Reset();
//    _timer.Start();
//    _entityRecalculateCount = 0;
//    foreach (PivotObject obj in objects)
//    {
//        if (obj.moved)
//        {
//            
//            /*  int hashCode = obj.GetHashCode();
//             bool needupdate = false;
//             if (_objectNodeMap.ContainsKey(hashCode))
//             {
//             MAXSceneGraphNode node = _objectNodeMap[hashCode];
//             ContainmentType type;
//             node.boundingRect.Contains(ref obj.raycastaspect.boundingShape.aabb.XNAbb, out type);
//             if (type != ContainmentType.Contains)
//             needupdate = true;
//             }
//             if (needupdate)
//             {*/
//            _entityRecalculateCount++;
//            RemoveObject(obj);
//            AddObject(obj);
//            // }
//        }
//    }
//    
//    _timer.Stop();
//    _updateTime = _timer.ElapsedMilliseconds;
//}
//
//
//void MAXSceneGraph::Clear()
//{
//    foreach (MAXObject lo in _scene._objects)
//    {
//        _octree.RemoveObject(lo);
//    }
//    //objects.Clear();
//}
