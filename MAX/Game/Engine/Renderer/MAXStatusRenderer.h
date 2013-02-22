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
    
    
    USimpleContainer<MAXUnitObject*> *_scanUnits;
    USimpleContainer<MAXUnitObject*> *_rangeUnits;
    
    
    bool _drawScan;
    bool _drawRange;
    bool _drawHealStatus;
    bool _drawShots;
    
    
    MAXStatusRenderer();
    ~MAXStatusRenderer();
    
    void Draw(USimpleContainer<MAXUnitObject*> visibleObjects);
    
    static MAXStatusRenderer* SharedStatusRenderer();
    
};

#endif /* defined(__MAX__MAXStatusRenderer__) */
