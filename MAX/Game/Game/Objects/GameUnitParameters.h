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
class MAXClanConfig;
class PlayerResearchManager;
class PlayerUpgradeManager;

class BinaryReader;

class GameUnitParameters {
    
    MAXUnitConfig            *_wconfig;
    MAXClanConfig            *_wclanBonuses;
    PlayerResearchManager    *_wresearchManager;
    PlayerUpgradeManager     *_wupgradeManager;
    
public:
    
    GameUnitParameters(MAXUnitConfig            *config,
                       MAXClanConfig            *clanBonuses,
                       PlayerResearchManager    *researchManager,
                       PlayerUpgradeManager     *upgradeManager);
    
    GameUnitParameters(BinaryReader             *saveReader);
    ~GameUnitParameters();
    
    void Upgrade();
};

#endif /* defined(__MAX__GameUnitParameters__) */
