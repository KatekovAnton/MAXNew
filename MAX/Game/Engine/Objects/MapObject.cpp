//
//  MapObject.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MapObject.h"
#include "MAXContentMap.h"
#include "MAXContetnLoader.h"
#include "MapRenderObject.h"
#include "MAXMapMaterial.h"

MapObject::MapObject(shared_ptr<MAXContentMap> map):PivotObject()
{
    _renderObject = new MapRenderObject(map);
    _material = new MAXMapMaterial(map);
    
    mapH = map->h;
    mapW = map->w;
    mapTexH = _material->texH;
    mapTexW = _material->texW;
}

MapObject::~MapObject()
{
    delete _renderObject;
    delete _material;
}

RenderObject * MapObject::GetRenderAspect()
{
    return _renderObject;
}

Material * MapObject::GetMaterial()
{
    return _material;
}

void MapObject::Frame(double time)
{
    _material->DoFrame(time);
}