//
//  DisplayPinchDelegate.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__DisplayPinchDelegate__
#define __MAX__DisplayPinchDelegate__

#include <iostream>

class DisplayPinchDelegate {
    
public:
    virtual bool CanStartPinch();
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
};

#endif /* defined(__MAX__DisplayPinchDelegate__) */
