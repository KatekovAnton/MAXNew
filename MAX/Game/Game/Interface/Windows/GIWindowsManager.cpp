//
//  GIWindowsManager.cpp
//  MAX
//
//  Created by  Developer on 27.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIWindowsManager.h"
#include "miniPrefix.h"
#include "GIWindow.h"

GIWindowsManager::GIWindowsManager(CCNode *parentNode)
:_baseNode(parentNode), _currentWindow(NULL)
{
    _baseNode->retain();
}

GIWindowsManager::~GIWindowsManager()
{
    _baseNode->release();
}

void GIWindowsManager::ProcessQueue()
{}

void GIWindowsManager::PresentWindow(GIWindow *window)
{
    if (_currentWindow) {
        _windowQueue.push_back(window);
        return;
    }
}

void GIWindowsManager::CloseCurrentWindow()
{
    if (!_currentWindow) {
        return;
    }
}
