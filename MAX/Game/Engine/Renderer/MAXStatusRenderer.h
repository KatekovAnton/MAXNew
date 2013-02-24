//
//  MAXStatusRenderer.h
//  MAX
//
//  Created by Anton Katekov on 22.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXStatusRenderer__
#define __MAX__MAXStatusRenderer__

#include <iostream>
#include "miniPrefix.h"

class MAXUnitObject;

class MAXStatusRenderer
{
    
public:
    
    
    vector<MAXUnitObject*> _scanRangeUnits_w;
    
    
    bool _drawScan;
    bool _drawRange;
    bool _drawHealStatus;
    bool _drawShots;
    
    
    MAXStatusRenderer();
    ~MAXStatusRenderer();
    
    void DrawUnitStatus(MAXUnitObject* unit);
    void DrawCircles();
    
    void SetLockedUnits(vector<MAXUnitObject*> objects);
    void ClearLockedUnits();
    void AddUnitToLock(MAXUnitObject* object);
    void RemoveUnitFromLock(MAXUnitObject* object);
    
    static MAXStatusRenderer* SharedStatusRenderer();
    
};

#endif /* defined(__MAX__MAXStatusRenderer__) */
