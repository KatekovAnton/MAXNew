//
//  MAXMapObject.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXMapObject.h"
#include "MAXContentMap.h"
#include "MAXContetnLoader.h"
#include "MapRenderObject.h"
#include "MAXMapMaterial.h"

MAXMapObject::MAXMapObject(shared_ptr<MAXContentMap> map):PivotObject()
{
    _renderObject = new MapRenderObject(map);
    _material = new MAXMapMaterial(map);
    
    mapH = map->h;
    mapW = map->w;
    mapTexH = _material->texH;
    mapTexW = _material->texW;
}

MAXMapObject::~MAXMapObject()
{
    delete _renderObject;
    delete _material;
}

RenderObject * MAXMapObject::GetRenderAspect()
{
    return _renderObject;
}

Material * MAXMapObject::GetMaterial()
{
    return _material;
}

void MAXMapObject::Frame(double time)
{
    _material->DoFrame(time);
}