//
//  GameSettings.h
//  MAX
//
//  Created by Alexey Malyshko on 3/26/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameSettings__
#define __MAX__GameSettings__

#include <iostream>
#include "GameMapResources.h"

class GameSettings
{
    void SetDebugSettings(void);
public:
    RESOURCE_LEVEL  resLevels[RESOURCE_TYPE_COUNT];     // Resources levels 
    int             resCount;                           // Total count of resource placements
    GameSettings();
};


#endif /* defined(__MAX__GameSettings__) */
