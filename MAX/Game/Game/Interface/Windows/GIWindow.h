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

class GIWindowPart : public CCNode {
    
public:
    
    bool _isLeft;
    float _startAlignment;
    float _finishAlignment;
    
    GIWindowPart();
    ~GIWindowPart();
    
};

class GIWindow {
    
public:
    
    vector<GIWindowPart*> _parts;
    
    GIWindow();
    ~GIWindow();
    
    virtual vector<float> RailAltitudes() = 0;
    
    
};

#endif /* defined(__MAX__GIWindow__) */
