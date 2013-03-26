//
//  GameSettings.cpp
//  MAX
//
//  Created by Alexey Malyshko on 3/26/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameSettings.h"

GameSettings::GameSettings()
{
    // default settings
    resLevels[RESOURCE_TYPE_RAW]  = RESOURCE_LEVEL_RICH;
    resLevels[RESOURCE_TYPE_FUEL] = RESOURCE_LEVEL_RICH;
    resLevels[RESOURCE_TYPE_GOLD] = RESOURCE_LEVEL_RICH;
    resCount = 160;

    SetDebugSettings();
}

void GameSettings::SetDebugSettings(void)
{
    resLevels[RESOURCE_TYPE_RAW]  = RESOURCE_LEVEL_RICH;
    resLevels[RESOURCE_TYPE_FUEL] = RESOURCE_LEVEL_MEDIUM;
    resLevels[RESOURCE_TYPE_GOLD] = RESOURCE_LEVEL_MEDIUM;
    resCount = 350;
}
