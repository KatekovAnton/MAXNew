//
//  MAXAnimationManager.cpp
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXAnimationManager.h"
#include "MAXEngine.h"

MAXAnimationManager::MAXAnimationManager()
{
    _objects = new UContainer<MAXAnimationObjectBase>();
}

MAXAnimationManager::~MAXAnimationManager()
{
    delete _objects;
}

void MAXAnimationManager::AddAnimatedObject(shared_ptr<MAXAnimationObjectBase>& object)
{}

void MAXAnimationManager::RemoveAnimatedObject(shared_ptr<MAXAnimationObjectBase>& object)
{}

void MAXAnimationManager::Update()
{
    double time = engine->ElapsedTime();
}