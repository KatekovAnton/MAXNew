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
    
    MAXObjectConfig            *_config_w;
    MAXClanConfig            *_clanBonuses_w;
    PlayerResearchManager    *_researchManager_w;
    PlayerUpgradeManager     *_upgradeManager_w;
    
public:
    
    MAXObjectConfig* GetCongig() const {return _config_w;}
    
    int _pAttack;
    int _pSpeed;
    int _pHealth;
    int _pArmor;
    int _pShots;
    int _pFuel;
    int _pRange;
    int _pScan;
    int _pAmmo;
    int _pCost;
    
    int _version;
    
    GameUnitParameters(MAXObjectConfig            *config,
                       MAXClanConfig            *clanBonuses,
                       PlayerResearchManager    *researchManager,
                       PlayerUpgradeManager     *upgradeManager);
    
    GameUnitParameters(BinaryReader             *saveReader);
    ~GameUnitParameters();
    
    void CalcParams(int researchArea, bool &incrementVersion);
    
    void Upgrade();
};

#endif /* defined(__MAX__GameUnitParameters__) */
