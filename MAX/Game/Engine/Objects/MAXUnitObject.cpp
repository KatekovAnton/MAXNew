//
//  MAXUnitObject.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXUnitObject.h"
#include "MAXUnitMaterial.h"
#include "MAXUnitRenderObject.h"

MAXUnitObject::MAXUnitObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material)
{
    
}

MAXUnitObject::~MAXUnitObject()
{}

RenderObject * MAXUnitObject::GetRenderAspect()
{
    return _renderAspect;
}

Material * MAXUnitObject::GetMaterial()
{
    return _material;
}

void MAXUnitObject::Frame(double time)
{}