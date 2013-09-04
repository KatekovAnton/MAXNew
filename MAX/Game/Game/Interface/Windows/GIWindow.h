//
//  GIWindow.h
//  MAX
//
//  Created by  Developer on 08.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GIWindow__
#define __MAX__GIWindow__

#include <iostream>
#include "miniPrefix.h"

class GIWindow : public CCNode {
    
public:
    
    GIWindow();
    virtual ~GIWindow();
    
    
    virtual void WindowWillAppear();
    virtual void WindowWillDisapper();
    
    virtual void WindowDidAppear();
    virtual void WindowDidDisapper();
    
    static CCSize GetMaximumSize();
    
};

#endif /* defined(__MAX__GIWindow__) */
