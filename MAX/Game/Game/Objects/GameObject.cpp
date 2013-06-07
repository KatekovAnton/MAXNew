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

