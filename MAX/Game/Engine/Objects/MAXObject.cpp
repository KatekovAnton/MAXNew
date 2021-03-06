//
//  MAXObject.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXObject.h"
#include "MAXEngine.h"
#include "MAXMapObject.h"
#include "MAXObjectConfig.h"

int compareMAXUnitObject (const void * a, const void * b)
{
    MAXObject* a1 = *((MAXObject**)a);
    MAXObject* b1 = *((MAXObject**)b);
    
    if (a1->_currentLevel == b1->_currentLevel) {
        float a1x = a1->GetTransformMatrix().m30;
        float a1y = a1->GetTransformMatrix().m31;
        float b1x = b1->GetTransformMatrix().m30;
        float b1y = b1->GetTransformMatrix().m31;
        if (a1x > b1x || a1y > b1y)//a1 before b1
            return -1;
        else
            return 1;
    }
    else if(a1->_currentLevel < b1->_currentLevel)
        return -1;
    else
        return 1;
    
    
    return 0;
}

compareFunc MAXObject::GetCompareFunc()
{
    return &compareMAXUnitObject;
}

MAXObject::MAXObject(MAXObjectConfig* params)
:params_w(params), _drawInLowLod(false), _currentLevel(params->_bLevel)
{}

MAXObject::~MAXObject()
{}

void MAXObject::SetPosition(const CCPoint &cell)
{
    GLKMatrix4 matrix = MAXObject::MatrixForCell(cell);
    this->SetGlobalPosition(matrix, NULL, NULL, true);
    OnPositionChanged();
//    CCPoint pt = ObjectCell();
//    pt = pt;
}

GLKMatrix4 MAXObject::MatrixForCell(const CCPoint& cell)
{
    GLKMatrix4 rt = GLKMatrix4MakeTranslation((cell.x - engine->_map->mapW/2.0) + 1, (-1*(cell.y + 1) + engine->_map->mapH/2.0), 0);
    return rt;
}

CCPoint MAXObject::GetObjectCell() const
{
    CCPoint result;
    GLKMatrix4 transform = GetTransformMatrix();
    result.x = transform.m30 - 1 + engine->_map->mapW/2;
    result.y = -1 * (transform.m31 - engine->_map->mapH/2) - 1;
    return result;
}

CCPoint MAXObject::GetObjectCoordinates() const
{
    CCPoint result;
    GLKMatrix4 transform = GetTransformMatrix();
    result.x = 64.0 * (transform.m30 - 1 + engine->_map->mapW/2);
    result.y = 64.0 * (-1.0 * (transform.m31 - engine->_map->mapH/2) - 1);
    return result;
}

CCPoint MAXObject::GetObjectCenterCoordinates() const
{
    CCPoint result;
    GLKMatrix4 transform = GetTransformMatrix();
    result.x = 64.0 * (transform.m30 - 1 + engine->_map->mapW/2) + 32.0;
    result.y = 64.0 * (-1.0 * (transform.m31 - engine->_map->mapH/2) - 1) + 32.0;
    return result;
}

CCPoint MAXObject::GetObjectCenterCoordinatesFromMatrix(GLKMatrix4 matr) const
{
    CCPoint result;
    result.x = 64.0 * (matr.m30 - 1 + engine->_map->mapW/2) + 32.0;
    result.y = 64.0 * (-1.0 * (matr.m31 - engine->_map->mapH/2) - 1) + 32.0;
    return result;
}

GLKVector2 MAXObject::GetDeltaPosition() const
{
    return GLKVector2Make(0, 0);
}

int MAXObject::CalculateRocketImageIndex(const CCPoint& cellLocation, const CCPoint& cellTarget)
{
    CCPoint delta = CCPoint(floorf(cellTarget.x) - floorf(cellLocation.x), floorf(cellTarget.y) - floorf(cellLocation.y));
    float l = sqrtf(delta.x * delta.x + delta.y * delta.y);
    delta.x = delta.x/l;//cos
    delta.y = delta.y/l;//sin
    
    int result = 0;
    
    if (delta.y > 0.980785)
        result = 8;//4
    else if(delta.y <= 0.980785 && delta.y > 0.831469)
        result = delta.x > 0?7:9;
    else if(delta.y <= 0.831469 && delta.y > 0.55557)
        result = delta.x > 0?6:10;
    else if(delta.y <= 0.55557 && delta.y > 0.19509)
        result = delta.x > 0?5:11;
    else if(delta.y <= 0.19509 && delta.y > -0.19509)
        result = delta.x > 0?4:12;
    else if(delta.y <= -0.19509 && delta.y > -0.55557)
        result = delta.x > 0?3:13;
    else if(delta.y <= -0.55557 && delta.y > -0.831469)
        result = delta.x > 0?2:14;
    else if(delta.y <= -0.831469 && delta.y > -0.980785)
        result = delta.x > 0?1:15;
    else if(delta.y < -0.980785)
        result = 0;
    
    return result;
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

