//
//  MAXObject.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXObject.h"


void MAXObject::SetPosition(const CCPoint& cell)
{
    GLKMatrix4 matrix = MAXObject::MatrixForCell(cell);
    this->SetGlobalPosition(matrix, NULL, NULL, true);
}

GLKMatrix4 MAXObject::MatrixForCell(const CCPoint& cell)
{
    GLKMatrix4 rt = GLKMatrix4MakeTranslation((cell.x - 112/2) + 1, ((-1*cell.y - 1) + 112/2), 0);
    return rt;
}

int MAXObject::CalculateImageIndex(const CCPoint& cellLocation, const CCPoint& cellTarget)
{
    CCPoint delta = CCPoint(floorf(cellTarget.x) - floorf(cellLocation.x), floorf(cellTarget.y) - floorf(cellLocation.y));
    float l = sqrtf(delta.x * delta.x + delta.y * delta.y);
    delta.x = delta.x/l;//cos
    delta.y = delta.y/l;//sin
    
    int result = 0;
    
    if (delta.y > 0.9238795)
        result = 4;
    else if(delta.y <= 0.923879 && delta.y > 0.3826834)
        result = delta.x > 0?3:5;
    else if(delta.y <= 0.3826834 && delta.y > -0.3826834)
        result = delta.x > 0?2:6;
    else if(delta.y <= -0.3826834 && delta.y > -0.923879)
        result = delta.x > 0?1:7;
    else if(delta.y < -0.923879)
        result = 0;
    
    
    
    return result;
}