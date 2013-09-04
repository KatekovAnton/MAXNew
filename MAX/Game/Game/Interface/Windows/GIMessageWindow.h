//
//  GIMessageWindow.h
//  MAX
//
//  Created by Anton Katekov on 04.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIMessageWindow__
#define __MAX__GIMessageWindow__

#include <iostream>
#include "GIWindow.h"
#include "GIMessageWindowDelegate.h"

class GIMessageWindow : public GIWindow {
    
    void OnButton(CCObject* sender);
    
public:
    
    GIMessageWindowDelegate *_delegate_w;
    
    GIMessageWindow(string title, string message, vector<string> buttons);
    virtual ~GIMessageWindow();

};

#endif /* defined(__MAX__GIMessageWindow__) */
