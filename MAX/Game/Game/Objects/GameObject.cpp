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

GameObject::GameObject(MAXObject* object, MAXObjectConfig* config)
:_object(object), _onMap(false), _config_w(config)
{}

GameObject::~GameObject()
{
    delete _object;
}

void GameObject::LocateOnMap()
{
    if (_onMap)
        return;
    _onMap = true;
    
    engine->AddUnit(_object);
}

void GameObject::RemoveFromMap()
{
    if (!_onMap)
        return;
    _onMap = false;
    
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

bool GameObject::IsUnitInCell(const CCPoint &cell) const
{
    if (_config_w->_bSize == 1)
        return (int)cell.x == (int)_unitCell.x && (int)cell.y == (int)_unitCell.y;
    else
    {
        return (cell.x>=_unitCell.x) &&
        (cell.y>=_unitCell.y) &&
        (cell.x - _unitCell.x <= _config_w->_bSize-1.0) &&
        (cell.y - _unitCell.y <= _config_w->_bSize-1.0);
    }
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

BoundingBox GameObject::GetScanBoundingBox(const CCPoint &point) const
{
    return GetBoundingBox(point, _config_w->_pScan);
}

BoundingBox GameObject::GetCurrentScanBoundingBox() const
{
    return GetScanBoundingBox(_unitCell);
}

