//
//  GameObject.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameObject.h"
#include "MAXObject.h"
#include "MAXEngine.h"
#include "MAXAnimationPrefix.h"
#include "MAXObjectConfig.h"
#include "miniPrefix.h"

GameObject::GameObject(MAXObject *object, MAXObjectConfig *config)
:_object(object), _onDraw(false), _config_w(config), selectedGameObjectDelegate(NULL)
{}

GameObject::~GameObject()
{
    delete _object;
}

void GameObject::Show()
{
    if (_onDraw)
        return;
    _onDraw = true;
    
    engine->AddUnit(_object);
}

void GameObject::Hide()
{
    if (!_onDraw)
        return;
    _onDraw = false;
    
    engine->RemoveUnit(_object);
}

void GameObject::SetLocation(const CCPoint &destination)
{
    _unitCell = destination;
    if (_config_w->_isBuilding && _config_w->_bSize == 2)
        _object->SetPosition(ccp(destination.x + 0.5, destination.y + 0.5));
    else
        _object->SetPosition(_unitCell);
}

// return some box for the object if the one is located in the point
BoundingBox GameObject::GetBoundingBox(const CCPoint &point, const float radius) const
{
    int unitSize = _config_w->_bSize;
    
    // count borders with size corrections
    return BoundingBoxMake(
        point.x - radius + (unitSize - 1),
        point.y - radius + (unitSize - 1),
        point.x + radius + (unitSize > 1 ? (unitSize - 2) : 0),
        point.y + radius + (unitSize > 1 ? (unitSize - 2) : 0)
    );
}

// return some box for the object for the current location
BoundingBox GameObject::GetCurrentBoundingBox(const float radius) const
{
    return GetBoundingBox(_unitCell, radius);
}

// check if point is in radius around the object
bool GameObject::IsInRadius(const CCPoint &point, const float radius) const
{
    float centerX = _unitCell.x;
    float centerY = _unitCell.y;
    
    // find the center for big objects
    if (_config_w->_bSize > 1)
    {
        centerX += 0.5 * _config_w->_bSize;
        centerY += 0.5 * _config_w->_bSize;
    }

    float distance = (point.x - centerX) * (point.x - centerX)
            + (point.y - centerY) * (point.y - centerY);
    
    return (radius * radius) >= distance;
}

// check if point is in radius around the object
bool GameObject::IsInRadius(const CCPoint &point, const float radius, const CCPoint &currentCenter) const
{
    float centerX = currentCenter.x;
    float centerY = currentCenter.y;
    
    // find the center for big objects
    if (_config_w->_bSize > 1)
    {
        centerX += 0.5 * _config_w->_bSize - 0.5;
        centerY += 0.5 * _config_w->_bSize - 0.5;
    }
    
    float distance = (point.x - centerX) * (point.x - centerX)
    + (point.y - centerY) * (point.y - centerY);
    
    return (radius * radius) >= distance;
}
