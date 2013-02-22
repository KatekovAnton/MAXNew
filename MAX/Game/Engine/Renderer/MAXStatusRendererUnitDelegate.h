//
//  MAXStatusRendererUnitDelegate.h
//  MAX
//
//  Created by Anton Katekov on 22.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_MAXStatusRendererUnitDelegate_h
#define MAX_MAXStatusRendererUnitDelegate_h

class MAXStatusRendererUnitDelegate
{
public:
    virtual int GetScan() const = 0;
    virtual int GetRange() const = 0;
    virtual float GetHealStatus() const = 0;
    virtual float GetShots() const = 0;
};

#endif
