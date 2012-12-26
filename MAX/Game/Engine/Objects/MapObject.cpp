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

MapObject::MapObject(shared_ptr<MAXContentMap> map)
{}

MapObject::~MapObject()
{}

RenderObject * MapObject::GetRenderAspect()
{
    return _renderObject;
}

Material * MapObject::GetMaterial()
{
    return _material;
}

void MapObject::Frame(double time)
{}