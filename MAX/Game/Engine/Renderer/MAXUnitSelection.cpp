//
//  MAXUnitSelection.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXUnitSelection.h"
#include "MAXEngine.h"
#include "MAXDrawPrimitives.h"
#include "cocos2d.h"
#include "MAXUnitObject.h"

using namespace cocos2d;

const double animSelectionTime = 0.15;

const float startD = 50.0;
const float endD = 28.0;
const float sideLength = 15;

//
//  1 2---3 4
//  0       5
//  |       |
//  11      6
//  10 9__8 7
//

MAXUnitSelection::MAXUnitSelection()
:_unitSelected(false)
{}

MAXUnitSelection::~MAXUnitSelection()
{}

void MAXUnitSelection::SelectUnit(MAXObject* unit)
{
    if (unit)
    {
        _animStartTime = 0;
        _unitSelected = true;
        _selectedUnit_w = unit;
    }
    else
    {
        DeselectUnit(_selectedUnit_w);
    }
}

void MAXUnitSelection::DeselectUnit(MAXObject* unit)
{
    _selectedUnit_w = NULL;
    _unitSelected = false;
}

void MAXUnitSelection::Update()
{
    if(!_unitSelected)
        return;
    if (_animStartTime == 0)
    {
        _animStartTime = engine->FullTime();
    }
    double time = engine->FullTime()-_animStartTime;
    _part = time/animSelectionTime;
    if(_part > 1)
        _part = 1;
    _part = 1.0 - cosf(_part * M_PI_2);
    
    
    float deltaD = startD + _part * (endD - startD);
    if (_selectedUnit_w->params_w->_bSize == 2)
        deltaD *= 2.0;
    
    //deltaD *= 0.5;
    //GLKMatrix4 matrix = _selectedUnit->GetTransformMatrix();
    CCPoint point = _selectedUnit_w->GetObjectCell();
    GLKVector2 delta = _selectedUnit_w->GetDeltaPosition();
    point.x += delta.x;
    point.y -= delta.y;
    
    point.x = point.x * 64.0 + 32.0;
    point.y = point.y * 64.0 + 32.0;
    _globalPoints[1].x = point.x - deltaD; _globalPoints[1].y = point.y - deltaD;
    _globalPoints[4].x = point.x + deltaD; _globalPoints[4].y = point.y - deltaD;
    _globalPoints[7].x = point.x + deltaD; _globalPoints[7].y = point.y + deltaD;
    _globalPoints[10].x = point.x - deltaD; _globalPoints[10].y = point.y + deltaD;
    
    
    _globalPoints[0].x = _globalPoints[1].x;
    _globalPoints[0].y = _globalPoints[1].y + sideLength;
    _globalPoints[2].x = _globalPoints[1].x + sideLength;
    _globalPoints[2].y = _globalPoints[1].y;
    
    _globalPoints[3].x = _globalPoints[4].x - sideLength;
    _globalPoints[3].y = _globalPoints[4].y;
    _globalPoints[5].x = _globalPoints[4].x;
    _globalPoints[5].y = _globalPoints[4].y + sideLength;
    
    _globalPoints[6].x = _globalPoints[7].x;
    _globalPoints[6].y = _globalPoints[7].y - sideLength;
    _globalPoints[8].x = _globalPoints[7].x - sideLength;
    _globalPoints[8].y = _globalPoints[7].y;
    
    _globalPoints[9].x = _globalPoints[10].x + sideLength;
    _globalPoints[9].y = _globalPoints[10].y;
    _globalPoints[11].x = _globalPoints[10].x;
    _globalPoints[11].y = _globalPoints[10].y - sideLength;
    
    
    for (int i = 0; i < 12; i++)
        _screenpoints[i] = engine->WorldCoordinatesToScreenCocos(_globalPoints[i]);
    
}

void MAXUnitSelection::Draw()
{
    if(!_unitSelected)
        return;
    
    double time = engine->FullTime()-_animStartTime;
    time = time/animSelectionTime;
    if(time>1)
        time = 1;

    
    glBlendFunc(GL_ONE, GL_SRC_ALPHA);
    MAXDrawPrimitives::SharedDrawPrimitives()->Begin();
    MAXDrawPrimitives::SharedDrawPrimitives()->BindColor(GLKVector4Make(140.0/255.0, 158.0/255.0, 0, time*time));
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[0], _screenpoints[1]);
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[1], _screenpoints[2]);
    
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[3], _screenpoints[4]);
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[4], _screenpoints[5]);
    
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[6], _screenpoints[7]);
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[7], _screenpoints[8]);
    
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[9], _screenpoints[10]);
    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(_screenpoints[10], _screenpoints[11]);
}




