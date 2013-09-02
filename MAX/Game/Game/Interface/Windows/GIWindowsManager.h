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

class GIWindowsManager : public CCObject {
    
    CCNode              *_baseNode;
    CCMenu              *_menu;
    
    vector<GIWindow*>   _windowQueue;
    GIWindow            *_currentWindow;
    CCLayerColor        *_nodeBase;
    
    void ProcessQueue();
    
public:
    
    GIWindowsManager(CCNode *parentNode);
    virtual ~GIWindowsManager();
    
    void PresentWindow(GIWindow *window, float h, bool queue);
    void CloseCurrentWindow();
    
    void OnCloseFinished(CCObject* sender);
    
};

#endif /* defined(__MAX__GIWindowsManager__) */
