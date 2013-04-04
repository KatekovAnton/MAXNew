//
//  GameUnitParameters.cpp
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitParameters.h"
#include "MAXObjectConfig.h"
#include "MAXClanConfig.h"
#include "PlayerResearchManager.h"
#include "PlayerUpgradeManager.h"
#include "BinaryReader.h"

GameUnitParameters::GameUnitParameters(MAXObjectConfig            *config,
                                       MAXClanConfig            *clanBonuses,
                                       PlayerResearchManager    *researchManager,
                                       PlayerUpgradeManager     *upgradeManager)
:_config_w(config), _clanBonuses_w(clanBonuses), _researchManager_w(researchManager), _upgradeManager_w(upgradeManager), _version(1)
{
    bool iv;
    for (int i = 0; i < kNrResearchAreas; i++) 
        CalcParams(i, iv);
}

GameUnitParameters::GameUnitParameters(BinaryReader             *saveReader)
{}

void GameUnitParameters::CalcParams(int researchArea, bool &incrementVersion)
{
    int startValue = 0;
    switch (researchArea)
    {
        case kAttackResearch: startValue = _config_w->_pAttack; break;
        case kShotsResearch: startValue = _config_w->_pShots; break;
        case kRangeResearch: startValue = _config_w->_pRange; break;
        case kArmorResearch: startValue = _config_w->_pArmor; break;
        case kHitpointsResearch: startValue = _config_w->_pHealth; break;
        case kScanResearch: startValue = _config_w->_pScan; break;
        case kSpeedResearch: startValue = _config_w->_pSpeed; break;
        case kCostResearch: startValue = _config_w->_pCost; break;
    }
    int newResearchLevel = _researchManager_w->getCurResearchLevel (researchArea);
    int oldResearchBonus = _upgradeManager_w->calcChangeByResearch (startValue, newResearchLevel - 10,
                                                                                researchArea == kCostResearch ? kCost : -1,
                                                                                (_config_w->_isInfantry || _config_w->_isInfiltrator)  ? PlayerUpgradeManager::kInfantry : PlayerUpgradeManager::kStandardUnit);
    int newResearchBonus = _upgradeManager_w->calcChangeByResearch (startValue, newResearchLevel,
                                                                                researchArea == kCostResearch ? kCost : -1,
                                                                                (_config_w->_isInfantry || _config_w->_isInfiltrator) ? PlayerUpgradeManager::kInfantry : PlayerUpgradeManager::kStandardUnit);
    
    switch (researchArea)
    {
        case kAttackResearch: _pMaxAttack =    _config_w->_pAttack + newResearchBonus; break;
        case kShotsResearch: _pMaxShots =      _config_w->_pShots + newResearchBonus; break;
        case kRangeResearch: _pMaxRange =      _config_w->_pRange + newResearchBonus; break;
        case kArmorResearch: _pMaxArmor =      _config_w->_pArmor + newResearchBonus; break;
        case kHitpointsResearch: _pMaxHealth = _config_w->_pHealth + newResearchBonus; break;
        case kScanResearch: _pMaxScan =        _config_w->_pScan + newResearchBonus; break;
        case kSpeedResearch: _pMaxSpeed =      _config_w->_pSpeed + newResearchBonus; break;
        case kCostResearch: _pMaxCost =        _config_w->_pCost + newResearchBonus; break;
    }
    _pMaxAmmo = _config_w->_pAmmo;
    if ((researchArea != kCostResearch) && (oldResearchBonus != newResearchBonus))   // don't increment the version, if the only change are the costs
        incrementVersion = true;
    
}

GameUnitParameters::~GameUnitParameters()
{}

void GameUnitParameters::Upgrade()
{
    bool iv;
    for (int i = 0; i < kNrResearchAreas; i++) 
        CalcParams(i, iv);
    
    if (iv) 
        _version ++;
}

bool GameUnitParameters::GetIsSurvivor() const
{
    return _config_w->_isSurvivor;
}

bool GameUnitParameters::GetIsBuilding() const
{
    return _config_w->_isBuilding;
}

int GameUnitParameters::GetSize() const
{
    return _config_w->_bSize;
}


