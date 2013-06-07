//
//  GameMatchPlayerDataDelegate.h
//  MAX
//
//  Created by  Developer on 08.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GameMatchPlayerDataDelegate_h
#define MAX_GameMatchPlayerDataDelegate_h

#include "miniPrefix.h"

class GameMatchPlayerDataDelegate {
    
public:
    
    virtual EXTENDED_GROUND_TYPE GroudTypeAtPoint(const int x, const int y) = 0;
    virtual void CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag) = 0;
};

#endif
