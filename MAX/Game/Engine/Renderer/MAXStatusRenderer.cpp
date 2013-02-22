//
//  MAXStatusRenderer.cpp
//  MAX
//
//  Created by Anton Katekov on 22.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXStatusRenderer.h"

MAXStatusRenderer* _sharedMAXStatusRenderer = NULL;

MAXStatusRenderer::MAXStatusRenderer()
{}

MAXStatusRenderer::~MAXStatusRenderer()
{}

MAXStatusRenderer* MAXStatusRenderer::SharedStatusRenderer()
{
    if (!_sharedMAXStatusRenderer)
        _sharedMAXStatusRenderer = new MAXStatusRenderer();
    return _sharedMAXStatusRenderer;
}

void MAXStatusRenderer::Draw(USimpleContainer<MAXUnitObject*> visibleObjects)
{
    if (_drawScan)
    {}

    if (_drawRange)
    {}

    if (_drawHealStatus)
    {}

    if (_drawShots)
    {}
}