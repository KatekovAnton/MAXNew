//
//  MAXUnitSelection.h
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXUnitSelection__
#define __MAX__MAXUnitSelection__

#include <iostream>
#include "miniPrefix.h"

class MAXUnitObject;

class MAXUnitSelection {
    
    shared_ptr<MAXUnitObject> _selectedUnit;
    
    double _animStartTime;
    
public:
    
    MAXUnitSelection();
    ~MAXUnitSelection();
    
    void SelectUnit(shared_ptr<MAXUnitObject> unit);
    void DeelectUnit(shared_ptr<MAXUnitObject> unit);
    
    void Update();
    void Draw();
};

#endif /* defined(__MAX__MAXUnitSelection__) */