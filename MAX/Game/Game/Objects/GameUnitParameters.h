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

class MAXObjectConfig;
class MAXClanConfig;
class PlayerResearchManager;
class PlayerUpgradeManager;

class BinaryReader;

class GameUnitParameters {
    
    MAXObjectConfig       *_config_w;
    MAXClanConfig         *_clanBonuses_w;
    PlayerResearchManager *_researchManager_w;
    PlayerUpgradeManager  *_upgradeManager_w;
    
public:
    
    MAXObjectConfig* GetConfig() const {return _config_w;}
    
    int _pMaxAttack;
    int _pMaxSpeed;
    int _pMaxHealth;
    int _pMaxArmor;
    int _pMaxShots;
    int _pMaxFuel;
    int _pMaxRange;
    int _pMaxScan;
    int _pMaxAmmo;
    int _pMaxCost;
    
    int _version;
    
    GameUnitParameters(MAXObjectConfig       *config,
                       MAXClanConfig         *clanBonuses,
                       PlayerResearchManager *researchManager,
                       PlayerUpgradeManager  *upgradeManager);
    
    GameUnitParameters(BinaryReader          *saveReader);
    ~GameUnitParameters();
    
    void CalcParams(int researchArea, bool &incrementVersion);
    
    void Upgrade();
    
    bool GetIsSurvivor() const;
};

#endif /* defined(__MAX__GameUnitParameters__) */
