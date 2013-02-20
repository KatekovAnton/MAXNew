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
    
    MAXUnitObject* _selectedUnit_w;
    
    double _animStartTime;
    
    
    CCPoint _globalPoints[12];
    CCPoint _screenpoints[12];
    bool _unitSelected;
    float _part;
    
public:
    
    MAXUnitSelection();
    ~MAXUnitSelection();
    
    void SelectUnit(MAXUnitObject* unit);
    void DeselectUnit(MAXUnitObject* unit);
    
    void Update();
    void Draw();
};

#endif /* defined(__MAX__MAXUnitSelection__) */
