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

using namespace std;
using namespace cocos2d;

class MAXObject;
class MAXObjectConfig;

class GameObject {

protected:
    CCPoint _unitCell;
    MAXObject* _object;
    MAXObjectConfig* _config_w;
    
public:
    
    bool _onMap;
    
    GameObject(MAXObject* object, MAXObjectConfig* config);
    ~GameObject();
    
    MAXObject* GetObject() const {return _object;};
    CCPoint GetUnitCell() const {return _unitCell;};
    
    bool IsUnitInCell(const CCPoint &cell) const;
    
    virtual void SetLocation(const CCPoint& cell);
    virtual void LocateOnMap();
    virtual void RemoveFromMap();
};

#endif /* defined(__MAX__Object__) */
