//
//  PlayerUpgradeManager.h
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__PlayerUpgradeManager__
#define __MAX__PlayerUpgradeManager__

#include <iostream>
#include "miniPrefix.h"

enum UpgradeTypes
{
    kHitpoints = 0,
    kArmor,
    kAmmo,
    kAttack,
    kSpeed,
    kShots,
    kRange,
    kScan,
    kCost
};

enum
{
    kNoPriceAvailable = 0,
    kNoResearchAvailable = 66666
};

class PlayerResearchManager;
class GameMatchPlayer;


typedef map<int, int> PriceMap;

class PlayerUpgradeManager {
    
public:
    
    /** Calculates the price (gold) to upgrade from the given value.
     @param curValue the value the unit currently has (without boni by research!)
     @param orgValue the value the unit has as a base value
     @param upgradeType the area of the upgrade
     @param researchLevel the research level of the player that has to be taken into account
     @return the costs for this upgrade or kNoPriceAvailable if the values are unknown */
	int calcPrice (int curValue, int orgValue, int upgradeType) const;
    
	/** Calculates the increase of a unit value, when an upgrade is bought.
     Examples: If orgValue is 10, the increase will be 2.
     If orgValue is 28, the increase will be 5.
     The increase is not growing, if the unit has already some upgrades! The only
     needed thing for the calculation is the value, at which the unit started.
     @param startValue the value, the unit has in it's base version
     @return the increase of the unit's value, when an upgrade is bought
     */
	int calcIncreaseByUpgrade (int startValue) const;
    
	/** Calculates the price (gold) for upgrading a unit, that started with orgValue and has
     currently curValue, to newValue.
	 @param orgValue the value the unit has as a base value
	 @param curValue the value the unit currently has
	 @param newValue the value the unit wants to reach
	 @upgradeType the area of the upgrade
	 @param researchLevel the research level of the player that has to be taken into account
	 @return the costs for this upgrade or kNoPriceAvailable if such an upgrade is impossible
     */
	int getCostForUpgrade (int orgValue, int curValue, int newValue, int upgradeType) const;
    
	/** Calculates the turns needed for one research center to reach the next level.
	 @param curResearchLevel the level this research area currently has (e.g. 20 for 20%)
	 @param upgradeType the area of the upgrade
	 @return the turns needed to reach the next level with one research center or
     kNoResearchAvailable if the passed values are out of range */
	int calcResearchTurns (int curResearchLevel, int upgradeType) const;
    
	/** Calculates the raw-material needed for upgrading a unit, that costs unitCost, to the current version.
     The costs in original M.A.X. are simply a fourth of the costs (rounded down) needed to build that unit.
     The costs do not depend on the quality of the upgrade (e.g. upgrading hitpoints from 18 to 20 costs the
     same as upgrading the basic version of the unit to an ultra fat version with all values upgraded to a
     maximum).
	 @param unitCost the raw-material cost to build the unit that will be upgraded (e.g. 24 for a mine-building)
	 @return the raw-material needed to upgrade to the current version */
	int getMaterialCostForUpgrading (int unitCost) const;
    
	enum UnitTypes
	{
		kBuilding = 0, // Mines, Research Centers, Storage, Generators, ...
		kInfantry, // Infantry and Infiltrator
		kStandardUnit // all other, like Tank, Ground Attack Plane, Scanner, ...
	};
    
	/** Calculates the change of the given startValue, with the given researchLevel.
     This change is independent of the upgradeType, only kCost has a special handling,
     because it actually decreases the value (so you will get a negative value as
     return value).
     @param startValue the value, the unit has in it's base version
     @param curResearchLevel the level for which you want to know the change (e.g. 10 for 10%)
     @param upgradeType optional, set it to kCost if you need to know the changes in cost
     @param unitType optional, needed for upgradeType kCost because the
     behaviour changes for the unit types
     @return the change of the startValue (can be negative if kCost is the upgradeType) */
    int calcChangeByResearch (int startValue, int curResearchLevel,
							  int upgradeType = -1, int unitType = kBuilding) const;
    
    
	/** Prints some upgrade values to the standard log on debug-level. Expand the implementation
     to test, if all works fine. */
	void printAllToLog() const;
    
    
	//-------------------------------------------
	int lookupPrice (const PriceMap& prices, int value) const;
    
	int getNearestPossibleCost (double realCost, int costDifference) const;
    
	void printToLog (const char* str, int value = -1000) const;
    
    GameMatchPlayer* _player_w;
    
    PlayerUpgradeManager(GameMatchPlayer* player);
    ~PlayerUpgradeManager();
};

#endif /* defined(__MAX__PlayerUpgradeManager__) */
