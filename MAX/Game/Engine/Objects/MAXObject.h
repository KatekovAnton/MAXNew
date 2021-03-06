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

#define SHADOWALPHA 0.4

using namespace cocos2d;

class MAXObjectConfig;

class MAXObject : public PivotObject {
    
public:
    
    int _currentLevel;
    bool _drawInLowLod;
    
    
    MAXObjectConfig* params_w;
    
    MAXObject(MAXObjectConfig* params);
    virtual ~MAXObject();
    
    virtual void OnPositionChanged() {};
    
    void SetPosition(const CCPoint &cell);
    CCPoint GetObjectCell() const;
    CCPoint GetObjectCoordinates() const;
    CCPoint GetObjectCenterCoordinates() const;
    CCPoint GetObjectCenterCoordinatesFromMatrix(GLKMatrix4 matr) const;
    
    virtual GLKVector2 GetDeltaPosition() const;
    virtual compareFunc GetCompareFunc();
    
    static GLKMatrix4 MatrixForCell(const CCPoint& cell);
    static int CalculateRocketImageIndex(const CCPoint& cellLocation, const CCPoint& cellTarget);
    static int CalculateImageIndex(const CCPoint& cellLocation, const CCPoint& cellTarget);
    
};

#endif /* defined(__MAX__MAXObject__) */
