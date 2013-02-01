//
//  MAXUnitSelection.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXUnitSelection.h"
#include "MAXEngine.h"

const double animSelectionTime = 0.15;

MAXUnitSelection::MAXUnitSelection()
{}

MAXUnitSelection::~MAXUnitSelection()
{}

void MAXUnitSelection::SelectUnit(shared_ptr<MAXUnitObject> unit)
{
    _animStartTime = engine->FullTime();
}

void MAXUnitSelection::DeelectUnit(shared_ptr<MAXUnitObject> unit)
{}

void MAXUnitSelection::Update()
{
    double time = engine->FullTime();
    time = time;
}

void MAXUnitSelection::Draw()
{
}