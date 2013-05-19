//
//  GameUnitParameters.cpp
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitParameters.h"

#include "GameUnitBaseParameters.h"

#include "PlayerResearchManager.h"
#include "PlayerUpgradeManager.h"
#include "BinaryReader.h"

GameUnitParameters::GameUnitParameters(GameUnitBaseParameters   *config,
                                       PlayerResearchManager    *researchManager,
                                       PlayerUpgradeManager     *upgradeManager)
:_config_w(config), _researchManager_w(researchManager), _upgradeManager_w(upgradeManager), _version(1)
{
    bool iv;
    _pMaxFuel = config->GetPFuel();
    for (int i = 0; i < kNrResearchAreas; i++) 
        CalcParams(i, iv);
}

MAXObjectConfig* GameUnitParameters::GetConfig() const
{
    return _config_w->_configObject;
}

GameUnitParameters::GameUnitParameters(GameUnitBaseParameters            *config)
:_config_w(config), _researchManager_w(NULL), _upgradeManager_w(NULL), _version(0)
{

}

GameUnitParameters::GameUnitParameters(BinaryReader             *saveReader)
{}

void GameUnitParameters::CalcParams(int researchArea, bool &incrementVersion)
{
    int startValue = 0;
    switch (researchArea)
    {
        case kAttackResearch: startValue = _config_w->GetPAttack(); break;
        case kShotsResearch: startValue = _config_w->GetPShots(); break;
        case kRangeResearch: startValue = _config_w->GetPRange(); break;
        case kArmorResearch: startValue = _config_w->GetPArmor(); break;
        case kHitpointsResearch: startValue = _config_w->GetPHealth(); break;
        case kScanResearch: startValue = _config_w->GetPScan(); break;
        case kSpeedResearch: startValue = _config_w->GetPSpeed(); break;
        case kCostResearch: startValue = _config_w->GetPCost(); break;
    }
    int newResearchLevel = _researchManager_w->getCurResearchLevel (researchArea);
    int oldResearchBonus = _upgradeManager_w->calcChangeByResearch (startValue, newResearchLevel - 10,
                                                                                researchArea == kCostResearch ? kCost : -1,
                                                                                (_config_w->GetIsInfantry() || _config_w->GetIsInfiltrator())  ? PlayerUpgradeManager::kInfantry : PlayerUpgradeManager::kStandardUnit);
    int newResearchBonus = _upgradeManager_w->calcChangeByResearch (startValue, newResearchLevel,
                                                                                researchArea == kCostResearch ? kCost : -1,
                                                                                (_config_w->GetIsInfantry() || _config_w->GetIsInfiltrator()) ? PlayerUpgradeManager::kInfantry : PlayerUpgradeManager::kStandardUnit);
    
    switch (researchArea)
    {
        case kAttackResearch: _pMaxAttack =    _config_w->GetPAttack() + newResearchBonus; break;
        case kShotsResearch: _pMaxShots =      _config_w->GetPShots() + newResearchBonus; break;
        case kRangeResearch: _pMaxRange =      _config_w->GetPRange() + newResearchBonus; break;
        case kArmorResearch: _pMaxArmor =      _config_w->GetPArmor() + newResearchBonus; break;
        case kHitpointsResearch: _pMaxHealth = _config_w->GetPHealth() + newResearchBonus; break;
        case kScanResearch: _pMaxScan =        _config_w->GetPScan() + newResearchBonus; break;
        case kSpeedResearch: _pMaxSpeed =      _config_w->GetPSpeed() + newResearchBonus; break;
        case kCostResearch: _pMaxCost =        _config_w->GetPCost() + newResearchBonus; break;
    }
    _pMaxAmmo = _config_w->GetPAmmo();
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
    return _config_w->GetIsSurvivor();
}

bool GameUnitParameters::GetIsBuilding() const
{
    return _config_w->GetIsBuilding();
}

int GameUnitParameters::GetSize() const
{
    return _config_w->GetBSize();
}

int GameUnitParameters::GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const
{
    int result = 0;
    switch (parameterType)
    {
        case UNIT_PARAMETER_TYPE_SPEED:
            result = _pMaxSpeed;
            break;
        case UNIT_PARAMETER_TYPE_HEALTH:
            result = _pMaxHealth;
            break;
        case UNIT_PARAMETER_TYPE_ARMOR:
            result = _pMaxArmor;
            break;
        case UNIT_PARAMETER_TYPE_ATTACK:
            result = _pMaxAttack;
            break;
        case UNIT_PARAMETER_TYPE_SHOTS:
            result = _pMaxShots;
            break;
        case UNIT_PARAMETER_TYPE_GAS:
            result = _pMaxFuel;
            break;
        case UNIT_PARAMETER_TYPE_RANGE:
            result = _pMaxRange;
            break;
        case UNIT_PARAMETER_TYPE_SCAN:
            result = _pMaxScan;
            break;
        case UNIT_PARAMETER_TYPE_AMMO:
            result = _pMaxAmmo;
            break;
        case UNIT_PARAMETER_TYPE_COST:
            result = _pMaxCost;
            break;
            
        default:
            break;
    }
    return result;
}

