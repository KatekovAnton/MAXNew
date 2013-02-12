//
//  GameUnitParameters.h
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameUnitParameters__
#define __MAX__GameUnitParameters__

#include <iostream>

class MAXUnitConfig;
class PlayerResearchManager;
class PlayerUpgradeManager;
class PlayerClanBonuses;

class BinaryReader;

class GameUnitParameters {
    
public:
    GameUnitParameters(MAXUnitConfig            *config,
                       PlayerClanBonuses        *clanBonuses,
                       PlayerResearchManager    *researchManager,
                       PlayerUpgradeManager     *upgradeManager);
    GameUnitParameters(BinaryReader             *saveReader);
    ~GameUnitParameters();
};

#endif /* defined(__MAX__GameUnitParameters__) */
