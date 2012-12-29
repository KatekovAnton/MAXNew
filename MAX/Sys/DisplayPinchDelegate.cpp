//
//  DisplayPinchDelegate.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "DisplayPinchDelegate.h"

bool DisplayPinchDelegate::CanStartPinch(float x, float y)
{
    return false;
}

void DisplayPinchDelegate::ProceedPinch(float scale)
{}

void DisplayPinchDelegate::ProceedPan(float speedx, float speedy)
{}

void DisplayPinchDelegate::ProceedTap(float tapx, float tapy)
{}

void DisplayPinchDelegate::ProceedLongTap(float tapx, float tapy)
{}
