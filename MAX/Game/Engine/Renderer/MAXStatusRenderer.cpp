//
//  MAXStatusRenderer.cpp
//  MAX
//
//  Created by Anton Katekov on 22.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXStatusRenderer.h"
#include "MAXStatusRendererUnitDelegate.h"
#include "MAXUnitObject.h"
#include "MAXDrawPrimitives.h"
#include "MAXEngine.h"
#include "MAXCamera.h"
#include "Display.h"

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

void MAXStatusRenderer::SetLockedUnits(vector<MAXUnitObject*> objects)
{
    _scanRangeUnits_w = objects;
}

void MAXStatusRenderer::ClearLockedUnits()
{
    _scanRangeUnits_w.clear();
}

void MAXStatusRenderer::AddUnitToLock(MAXUnitObject* object)
{
    _scanRangeUnits_w.push_back(object);
}

void MAXStatusRenderer::RemoveUnitFromLock(MAXUnitObject* object)
{
    bool finded = true;
    while (finded)
    {
        finded = false;
        vector<MAXUnitObject*>::iterator obj = find(_scanRangeUnits_w.begin(), _scanRangeUnits_w.end(), object);
        if (obj != _scanRangeUnits_w.end())
        {
            _scanRangeUnits_w.erase(obj);
            finded = true;
        }
    }
}

void MAXStatusRenderer::DrawUnitStatus(MAXUnitObject* unit)
{
    if (_drawHealStatus)
    {}
    
    if (_drawShots)
    {}
}
int lastPrinted = -1;
void MAXStatusRenderer::DrawCircles()
{
    MAXDrawPrimitives* drawer = MAXDrawPrimitives::SharedDrawPrimitives();
    
    CCPoint c1 = engine->WorldCoordinatesToScreenCocos(ccp(0, 0));
    CCPoint c2 = engine->WorldCoordinatesToScreenCocos(ccp(64, 0));
    float oneCellRadius = c2.x - c1.x;
    CCPoint center;
    
    glBlendFunc(GL_ONE, GL_SRC_ALPHA);
    drawer->Begin();
    if (_drawScan)
    {
        drawer->BindColor(GLKVector4Make(140.0/255.0, 158.0/255.0, 0, 1.0));
        for (vector<MAXUnitObject*>::iterator obj = _scanRangeUnits_w.begin(); obj != _scanRangeUnits_w.end(); obj ++)
        {
            MAXUnitObject* object = *obj;
            MAXStatusRendererUnitDelegate* delegate = object->_statusDelegate_w;
            center = engine->WorldCoordinatesToScreenCocos(object->ObjectCenterCoordinates());
            
            float scan = delegate->GetScan() - 0.5;
            drawer->DrawCircle(center, scan * oneCellRadius + oneCellRadius * 0.49, 0, 30 + delegate->GetScan(), false);
        }
    }
    
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    if (_drawRange)
    {
        drawer->BindColor(GLKVector4Make(1.0, 0, 0, 1.0));
        for (vector<MAXUnitObject*>::iterator obj = _scanRangeUnits_w.begin(); obj != _scanRangeUnits_w.end(); obj ++)
        {
            MAXUnitObject* object = *obj;
            if (!object->params_w->_isAbleToFire) 
                continue;
            
            
            MAXStatusRendererUnitDelegate* delegate = object->_statusDelegate_w;
            
            float range = delegate->GetRange() - 0.5;
            center = engine->WorldCoordinatesToScreenCocos(object->ObjectCenterCoordinates());
            drawer->DrawCircle(center, range * oneCellRadius + oneCellRadius * 0.52, 0, 30 + delegate->GetRange(), false);
        }
    }
}

