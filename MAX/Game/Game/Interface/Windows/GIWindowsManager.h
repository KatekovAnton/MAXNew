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
#include "CocosHelper.h"

class GIWindow;
class GIWindowPart;
class CCWaitExtended;
class CCSetFrameExtended;
class GIWindowsManagerDelegate;

class GIWindowsManager : public CCObject {
    
    CCNode              *_baseNode;
    CCMenu              *_menu;
    
    
    vector<GIWindow*>   _windowQueue;
    vector<GIWindow*>   _windowStack;
    
    CCLayerColor        *_nodeBlackBase;         //black background
    CCMenu              *_windowBlockMenu;  //for animation
    
    
    void ProcessQueue();
    void CloseBlackBase();
    
#pragma mark - animations and delays callbacks
    void OnBlackThingEnlaged(CCObject *sender);
    void OnWindowApperarAnimationFinished(CCObject *sender);
    void OnWindowDisapperarAnimationFinished(CCObject *sender);
    void OnCloseFinished(CCObject* sender);
    
public:
    
    GIWindowsManagerDelegate *_delegate_w;
    
    GIWindowsManager(CCNode *parentNode);
    virtual ~GIWindowsManager();
    
    void PresentWindow(GIWindow *window, float w, bool queue, float animDelay);
    void DisappearWindow(GIWindow* window);
    
    bool IsWindowOpened();
    
    static CCSize MaximumSize();
    
};

#endif /* defined(__MAX__GIWindowsManager__) */
