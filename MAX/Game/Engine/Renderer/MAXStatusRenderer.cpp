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
//    if (_scanRangeUnits_w.size() != lastPrinted)
//    {
//        lastPrinted = _scanRangeUnits_w.size();
//        printf("%d\n", lastPrinted);
//    }
    
    MAXDrawPrimitives* drawer = MAXDrawPrimitives::SharedDrawPrimitives();
    float oneCellRadius = (64.0 / engine->_camera->scale)/Display::currentDisplay()->GetDisplayScale();
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
            drawer->DrawCircle(center, delegate->GetScan() * oneCellRadius + oneCellRadius/2, 0, 30 + delegate->GetScan(), false);
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
            float mult = 0.5;
            if (delegate->GetRange() == delegate->GetScan()) 
                mult = 0.55;
            
            center = engine->WorldCoordinatesToScreenCocos(object->ObjectCenterCoordinates());
            drawer->DrawCircle(center, delegate->GetRange() * oneCellRadius + oneCellRadius * mult, 0, 30 + delegate->GetRange(), false);
        }
    }
}

