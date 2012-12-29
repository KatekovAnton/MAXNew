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
    virtual bool CanStartPinch(float x, float y);
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
    virtual void ProceedTap(float tapx, float tapy);
    virtual void ProceedLongTap(float tapx, float tapy);
};

#endif /* defined(__MAX__DisplayPinchDelegate__) */
