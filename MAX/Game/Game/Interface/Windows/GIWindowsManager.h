//
//  GIWindowsManager.h
//  MAX
//
//  Created by  Developer on 27.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIWindowsManager__
#define __MAX__GIWindowsManager__

#include <iostream>
#include "miniPrefix.h"

class GIWindow;
class GIWindowPart;

class GIWindowsManager {
    
    CCNode              *_baseNode;
    
    vector<GIWindow*>   _windowQueue;
    GIWindow            *_currentWindow;
    
    
    void ProcessQueue();
    
public:
    
    GIWindowsManager(CCNode *parentNode);
    ~GIWindowsManager();
    
    void PresentWindow(GIWindow *window);
    void CloseCurrentWindow();
    
};

#endif /* defined(__MAX__GIWindowsManager__) */
