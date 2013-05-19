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
#include "miniPrefix.h"

class MAXObjectConfig;
class MAXClanConfig;
class GameUnitBaseParameters;
class PlayerResearchManager;
class PlayerUpgradeManager;

class BinaryReader;

class GameUnitParameters {
    
    GameUnitBaseParameters   *_unitBaseParameters_w;
    PlayerResearchManager *_researchManager_w;
    PlayerUpgradeManager  *_upgradeManager_w;
    
public:
    
    MAXObjectConfig* GetConfig() const;
    
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
    
    GameUnitParameters(GameUnitBaseParameters *config);
    GameUnitParameters(GameUnitBaseParameters *config,
                       PlayerResearchManager *researchManager,
                       PlayerUpgradeManager  *upgradeManager);
    
    GameUnitParameters(BinaryReader          *saveReader);
    ~GameUnitParameters();
    
    void CalcParams(int researchArea, bool &incrementVersion);
    
    void Upgrade();
    
    bool GetIsSurvivor() const;
    bool GetIsBuilding() const;
    int GetSize() const;
    
    int GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const;
    
};

#endif /* defined(__MAX__GameUnitParameters__) */
