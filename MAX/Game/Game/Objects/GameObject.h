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

class GameObject {

protected:
    CCPoint _unitCell;
    MAXObject* _object;

public:
    
    bool _onMap;
    
    GameObject(MAXObject* object);
    ~GameObject();
    
    MAXObject* GetObject() const {return _object;};
    CCPoint GetUnitCell() const {return _unitCell;};
    
    void SetLocation(const CCPoint& cell);
    
    void LocateOnMap();
    void RemoveFromMap();
};

#endif /* defined(__MAX__Object__) */
