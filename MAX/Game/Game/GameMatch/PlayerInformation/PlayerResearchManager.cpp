//
//  PlayerResearchManager.cpp
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "PlayerResearchManager.h"
#include "PlayerUpgradeManager.h"
#include "GameMatchPlayer.h"

PlayerResearchManager::PlayerResearchManager(GameMatchPlayer* player)
{
    _player_w = player;
    init();
}

PlayerResearchManager::~PlayerResearchManager()
{}

void PlayerResearchManager::init()
{
	for (int i = 0; i < kNrResearchAreas; i++)
	{
		curResearchLevel[i] = 0;
		curResearchPoints[i] = 0;
		neededResearchPoints[i] = _player_w->_upgradeManager->calcResearchTurns (0, getUpgradeCalculatorUpgradeType (i));
	}
}

//---------------------------------------------------------------------
int PlayerResearchManager::getCurResearchLevel (int researchArea) const
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas)
		return curResearchLevel[researchArea];
	return 0;
}

//---------------------------------------------------------------------
int PlayerResearchManager::getCurResearchPoints (int researchArea) const
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas)
		return curResearchPoints[researchArea];
	return 0;
}

//---------------------------------------------------------------------
int PlayerResearchManager::getNeededResearchPoints (int researchArea) const
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas)
		return neededResearchPoints[researchArea];
	return kNoResearchAvailable;
}

//---------------------------------------------------------------------
int PlayerResearchManager::getRemainingTurns (int researchArea, int centersWorkingOn) const
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas && centersWorkingOn > 0)
	{
		int remainingPoints = getRemainingResearchPoints (researchArea);
		if (remainingPoints % centersWorkingOn == 0)
			return remainingPoints / centersWorkingOn;
		else
			return (remainingPoints / centersWorkingOn) + 1;
	}
	return 0;
}

//---------------------------------------------------------------------
void PlayerResearchManager::setCurResearchLevel (int researchLevel, int researchArea)
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas && researchLevel >= 0 && researchLevel % 10 == 0)
	{
		curResearchLevel[researchArea] = researchLevel;
		neededResearchPoints[researchArea] = _player_w->_upgradeManager->calcResearchTurns (researchLevel, getUpgradeCalculatorUpgradeType (researchArea));
		if (curResearchPoints[researchArea] >= neededResearchPoints[researchArea])
			curResearchPoints[researchArea] = 0;
	}
}

//---------------------------------------------------------------------
void PlayerResearchManager::setCurResearchPoints (int researchPoints, int researchArea)
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas && researchPoints >= 0 && researchPoints < neededResearchPoints[researchArea])
		curResearchPoints[researchArea] = researchPoints;
}

//---------------------------------------------------------------------
bool PlayerResearchManager::doResearch (int researchPoints, int researchArea)
{
	if (0 <= researchArea && researchArea <= kNrResearchAreas && researchPoints > 0)
	{
		curResearchPoints[researchArea] += researchPoints;
		if (curResearchPoints[researchArea] >= neededResearchPoints [researchArea])
		{
			curResearchPoints[researchArea] = 0;
			curResearchLevel[researchArea] += 10;
			neededResearchPoints[researchArea] = _player_w->_upgradeManager->calcResearchTurns (curResearchLevel[researchArea],
                                                                                                   getUpgradeCalculatorUpgradeType (researchArea));
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------
int PlayerResearchManager::getUpgradeCalculatorUpgradeType (int researchArea) const
{
	switch (researchArea)
	{
		case kHitpointsResearch: return kHitpoints;
		case kArmorResearch: return kArmor;
		case kAttackResearch: return kAttack;
		case kSpeedResearch: return kSpeed;
		case kShotsResearch: return kShots;
		case kRangeResearch: return kRange;
		case kScanResearch: return kScan;
		case kCostResearch: return kCost;
	}
	return 0;
}

//---------------------------------------------------------------------
int PlayerResearchManager::getResearchArea (int upgradeCalculatorType) const
{
	switch (upgradeCalculatorType)
	{
		case kHitpoints: return kHitpointsResearch;
		case kArmor: return kArmorResearch;
		case kAmmo: return -1;
		case kAttack: return kAttackResearch;
		case kSpeed: return kSpeedResearch;
		case kShots: return kShotsResearch;
		case kRange: return kRangeResearch;
		case kScan: return kScanResearch;
		case kCost: return kCostResearch;
	}
	return 0;
}