//
//  GameFogDelegate.h
//  MAX
//
//  Created by  Developer on 30.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameFogDelegate__
#define __MAX__GameFogDelegate__

#include <iostream>

class GameFog;
class GameUnit;

class GameFogDelegate {
    
public:
    
    virtual bool UnitShouldUpdateFog(const GameUnit *unit, const GameFog *fog) const = 0;
    virtual float UnitScanRadiusForFog(const GameUnit *unit, const GameFog *fog) const = 0;
    virtual void CellDidUpdate(const int cellX, const int cellY, const GameFog *fog, bool visibleFlag) const = 0;
    
};

#endif /* defined(__MAX__GameFogDelegate__) */
