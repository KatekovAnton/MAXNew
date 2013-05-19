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
:_unitBaseParameters_w(config), _researchManager_w(researchManager), _upgradeManager_w(upgradeManager), _version(1)
{
    bool iv;
    _pMaxFuel = config->GetPFuel();
    for (int i = 0; i < kNrResearchAreas; i++) 
        CalcParams(i, iv);
}

MAXObjectConfig* GameUnitParameters::GetConfig() const
{
    return _unitBaseParameters_w->_configObject;
}

GameUnitParameters::GameUnitParameters(GameUnitBaseParameters            *config)
:_unitBaseParameters_w(config), _researchManager_w(NULL), _upgradeManager_w(NULL), _version(0)
{

}

GameUnitParameters::GameUnitParameters(BinaryReader             *saveReader)
{}

void GameUnitParameters::CalcParams(int researchArea, bool &incrementVersion)
{
    int startValue = 0;
    switch (researchArea)
    {
        case kAttackResearch: startValue = _unitBaseParameters_w->GetPAttack(); break;
        case kShotsResearch: startValue = _unitBaseParameters_w->GetPShots(); break;
        case kRangeResearch: startValue = _unitBaseParameters_w->GetPRange(); break;
        case kArmorResearch: startValue = _unitBaseParameters_w->GetPArmor(); break;
        case kHitpointsResearch: startValue = _unitBaseParameters_w->GetPHealth(); break;
        case kScanResearch: startValue = _unitBaseParameters_w->GetPScan(); break;
        case kSpeedResearch: startValue = _unitBaseParameters_w->GetPSpeed(); break;
        case kCostResearch: startValue = _unitBaseParameters_w->GetPCost(); break;
    }
    int newResearchLevel = _researchManager_w->getCurResearchLevel (researchArea);
    int oldResearchBonus = _upgradeManager_w->calcChangeByResearch (startValue, newResearchLevel - 10,
                                                                                researchArea == kCostResearch ? kCost : -1,
                                                                                (_unitBaseParameters_w->GetIsInfantry() || _unitBaseParameters_w->GetIsInfiltrator())  ? PlayerUpgradeManager::kInfantry : PlayerUpgradeManager::kStandardUnit);
    int newResearchBonus = _upgradeManager_w->calcChangeByResearch (startValue, newResearchLevel,
                                                                                researchArea == kCostResearch ? kCost : -1,
                                                                                (_unitBaseParameters_w->GetIsInfantry() || _unitBaseParameters_w->GetIsInfiltrator()) ? PlayerUpgradeManager::kInfantry : PlayerUpgradeManager::kStandardUnit);
    
    switch (researchArea)
    {
        case kAttackResearch: _pMaxAttack =    _unitBaseParameters_w->GetPAttack() + newResearchBonus; break;
        case kShotsResearch: _pMaxShots =      _unitBaseParameters_w->GetPShots() + newResearchBonus; break;
        case kRangeResearch: _pMaxRange =      _unitBaseParameters_w->GetPRange() + newResearchBonus; break;
        case kArmorResearch: _pMaxArmor =      _unitBaseParameters_w->GetPArmor() + newResearchBonus; break;
        case kHitpointsResearch: _pMaxHealth = _unitBaseParameters_w->GetPHealth() + newResearchBonus; break;
        case kScanResearch: _pMaxScan =        _unitBaseParameters_w->GetPScan() + newResearchBonus; break;
        case kSpeedResearch: _pMaxSpeed =      _unitBaseParameters_w->GetPSpeed() + newResearchBonus; break;
        case kCostResearch: _pMaxCost =        _unitBaseParameters_w->GetPCost() + newResearchBonus; break;
    }
    _pMaxAmmo = _unitBaseParameters_w->GetPAmmo();
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
    return _unitBaseParameters_w->GetIsSurvivor();
}

bool GameUnitParameters::GetIsBuilding() const
{
    return _unitBaseParameters_w->GetIsBuilding();
}

int GameUnitParameters::GetSize() const
{
    return _unitBaseParameters_w->GetBSize();
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

