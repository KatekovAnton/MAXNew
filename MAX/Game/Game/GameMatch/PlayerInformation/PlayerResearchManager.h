//
//  PlayerResearchManager.h
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__PlayerResearchManager__
#define __MAX__PlayerResearchManager__

#include <iostream>

enum ResearchArea
{
    kAttackResearch = 0,
    kShotsResearch,
    kRangeResearch,
    kArmorResearch,
    kHitpointsResearch,
    kSpeedResearch,
    kScanResearch,
    kCostResearch,
    kNrResearchAreas
};

class GameMatchPlayer;

class PlayerResearchManager {
    
public:
    
    GameMatchPlayer* _player_w;
    
    bool doResearch (int researchPoints, int researchArea);
    
	int getCurResearchLevel (int researchArea) const;  ///< 0, 10, 20, 30, ...
	int getCurResearchPoints (int researchArea) const;  ///< Number of research-center turns the player invested in an area
	int getNeededResearchPoints (int researchArea) const;  ///< Number of research-center turns needed to reach the next level
	int getRemainingResearchPoints (int researchArea) const { return getNeededResearchPoints (researchArea) - getCurResearchPoints (researchArea); }
    
	int getRemainingTurns (int researchArea, int centersWorkingOn) const;  ///< returns the needed number of turns to reach the next level with the given nr of research centers
    
	void setCurResearchLevel (int researchLevel, int researchArea);  ///< will also set the neededResearchPoints if necessary
	void setCurResearchPoints (int researchPoints, int researchArea);  ///< if researchPoints >= neededResearchPoints, nothing will be done
    
	int getUpgradeCalculatorUpgradeType (int researchArea) const;
	int getResearchArea (int upgradeCalculatorType) const;
    
	void init();  ///< sets all research information to the initial values
    
	int curResearchLevel[kNrResearchAreas]; ///< 0, 10, 20, 30, ...
	int curResearchPoints[kNrResearchAreas]; ///< Numberr of research-center turns the player invested in an area
	int neededResearchPoints[kNrResearchAreas]; ///< Number of research-center turns needed to reach the next level (remainingResearchPoints == neededResearchPoints - curResearchPoints)
    
    PlayerResearchManager(GameMatchPlayer* player);
    ~PlayerResearchManager();
};

#endif /* defined(__MAX__PlayerResearchManager__) */
