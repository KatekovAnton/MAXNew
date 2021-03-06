//
//  MAXStatusRenderer.cpp
//  MAX
//
//  Created by Anton Katekov on 22.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXStatusRenderer.h"
#include "MAXUnitObjectDelegate.h"
#include "MAXUnitObject.h"
#include "MAXDrawPrimitives.h"
#include "MAXEngine.h"
#include "MAXCamera.h"
#include "Display.h"

MAXStatusRenderer* _sharedMAXStatusRenderer = NULL;

MAXStatusRenderer::MAXStatusRenderer()
	:_drawHealStatus(false), 
	_drawScan(false),
     _drawRange(false),
     _drawShots(false)
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
            MAXUnitObjectDelegate* delegate = object->_delegate_w;
            if (delegate->ShouldSkipThisUnit())
                continue;
			if (delegate->ShoudDrawFakeCircle())
			{
				center = engine->WorldCoordinatesToScreenCocos(delegate->GetFakeCenter());
			}
			else
			{
				center = engine->WorldCoordinatesToScreenCocos(object->GetObjectCenterCoordinatesFromMatrix(object->CalculateUnitCenterMatrix()));
			}
            float scan = delegate->GetScan() - 0.5;
            if (scan>0)
                drawer->DrawCircle(center, scan * oneCellRadius + oneCellRadius * 0.49, 0, 30 + delegate->GetScan(), false);
        }
    }
    
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    if (_drawRange)
    {
        for (vector<MAXUnitObject*>::iterator obj = _scanRangeUnits_w.begin(); obj != _scanRangeUnits_w.end(); obj ++)
        {
            MAXUnitObject* object = *obj;
            if (!object->params_w->_isAbleToFire) 
                continue;
            
            
            MAXUnitObjectDelegate* delegate = object->_delegate_w;
            
            if (delegate->ShouldSkipThisUnit())
                continue;
            
            FIRE_TYPE fireType = object->FireType();
            switch (fireType)
            {
                case FIRE_TYPE_Ground:
                    drawer->BindColor(GLKVector4Make(1.0, 0, 0, 1.0));
                    break;
                case FIRE_TYPE_Air:
                    drawer->BindColor(GLKVector4Make(1.0, 0.65, 0, 1.0));
                    break;
                case FIRE_TYPE_UnderWater:
                    drawer->BindColor(GLKVector4Make(0.0, 0.0, 1.0, 1.0));
                    break;
                default:
                    drawer->BindColor(GLKVector4Make(1.0, 0, 0, 1.0));
                    break;
            }
            
            float range = delegate->GetRange() - 0.5;
			if (delegate->ShoudDrawFakeCircle())
			{
				center = engine->WorldCoordinatesToScreenCocos(delegate->GetFakeCenter());
			}
			else
			{
				center = engine->WorldCoordinatesToScreenCocos(object->GetObjectCenterCoordinatesFromMatrix(object->CalculateUnitCenterMatrix()));
			}
            drawer->DrawCircle(center, range * oneCellRadius + oneCellRadius * 0.52, 0, 30 + delegate->GetRange(), false);
        }
    }
}

