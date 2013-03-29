//
//  GameObject.h
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameObject__
#define __MAX__GameObject__

#include <iostream>
#include "cocos2d.h"
#include "Geometry.h"

using namespace std;
using namespace cocos2d;

class MAXObject;
class MAXObjectConfig;

class GameObject {

protected:
    CCPoint _unitCell;
    MAXObject *_object;
    MAXObjectConfig *_config_w;
    
public:
    
    bool _onMap;
    
    GameObject(MAXObject *object, MAXObjectConfig *config);
    ~GameObject();
    
    MAXObject *GetObject() const {return _object;};
    CCPoint GetUnitCell() const {return _unitCell;};
    
    bool IsUnitInCell(const CCPoint &cell) const;
    
    BoundingBox GetBoundingBox(const CCPoint &point, const float radius) const;
    BoundingBox GetCurrentBoundingBox(const float radius) const;
    BoundingBox GetScanBoundingBox(const CCPoint &centerPoint) const;
    BoundingBox GetScanBoundingBox() const;
    
    bool IsInRadius(const CCPoint &point, const float radius) const;
    bool IsInRadius(const CCPoint &point, const float radius, const CCPoint &currentCenter) const;
    bool IsInScanRadius(const CCPoint &point) const;
    bool IsInScanRadius(const CCPoint &point, const CCPoint &currentCenter) const;
    bool IsInFireRadius(const CCPoint &point) const;
    bool IsInFireRadius(const CCPoint &point, const CCPoint &currentCenter) const;
    
    virtual void SetLocation(const CCPoint &cell);
    virtual void Show();
    virtual void Hide();
};

#endif /* defined(__MAX__Object__) */
