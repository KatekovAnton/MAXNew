//
//  MAXObject.h
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXObject__
#define __MAX__MAXObject__

#include <iostream>
#include "PivotObject.h"
#include "cocos2d.h"

using namespace cocos2d;

class MAXObject : public PivotObject {
    
public:
    
    void SetPosition(const CCPoint& cell);
    
    static GLKMatrix4 MatrixForCell(const CCPoint& cell);
    static int CalculateImageIndex(const CCPoint& cellLocation, const CCPoint& cellTarget);
    
};

#endif /* defined(__MAX__MAXObject__) */
