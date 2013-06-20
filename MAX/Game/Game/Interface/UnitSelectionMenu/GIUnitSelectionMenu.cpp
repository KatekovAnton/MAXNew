//
//  GIUnitSelectionMenu.cpp
//  MAX
//
//  Created by Anton Katekov on 19.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIUnitSelectionMenu.h"
#include "GameUnit.h"

GIUnitSelectionMenu::GIUnitSelectionMenu(const vector<GameUnit*> units)
:_units(units), _delegate_w(NULL)
{
    CCArray* arr = CCArray::create();
    initWithArray(arr);
}

GIUnitSelectionMenu::~GIUnitSelectionMenu()
{}

CCMenuItem *GIUnitSelectionMenu::CreateMenuItemWithUnit(GameUnit *unit)
{
    return NULL;
}

void GIUnitSelectionMenu::OnButton(CCObject* sender)
{}

