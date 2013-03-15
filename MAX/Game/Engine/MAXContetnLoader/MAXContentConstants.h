//
//  MAXContentConstants.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef MAX_MAXContentConstants_h
#define MAX_MAXContentConstants_h

#include "miniPrefix.h"

struct __Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
    bool IsNear(unsigned char a1, unsigned char a2) const
    {
        return MAX(a1, a2) - MIN(a1, a2)<3;
    }
    
    inline bool operator == (const __Color &color) const
    {
        return IsNear(r, color.r) && IsNear(g, color.g) && IsNear(b, color.b) && IsNear(a, color.a);
    }
    
} typedef Color;

#endif
