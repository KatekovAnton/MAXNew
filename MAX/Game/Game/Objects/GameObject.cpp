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

GameObject::GameObject(MAXObject* object)
:_object(object)
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

void GameObject::SetLocation(const CCPoint& destination)
{
    _unitCell = destination;
    _object->SetPosition(destination);

}