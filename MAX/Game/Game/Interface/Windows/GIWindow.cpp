//
//  GIWindow.cpp
//  MAX
//
//  Created by  Developer on 08.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIWindow.h"
#include "GIWindowsManager.h"

GIWindow::GIWindow()
{}

GIWindow::~GIWindow()
{}

void GIWindow::WindowWillAppear()
{}

void GIWindow::WindowWillDisapper()
{}

void GIWindow::WindowDidAppear()
{}

void GIWindow::WindowDidDisapper()
{}

CCSize GIWindow::GetMaximumSize()
{
    return GIWindowsManager::MaximumSize();
}
