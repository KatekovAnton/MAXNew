//
//  GameUnitParameters.cpp
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitParameters.h"
#include "MAXUnitConfig.h"
#include "MAXClanConfig.h"
#include "PlayerResearchManager.h"
#include "PlayerUpgradeManager.h"
#include "BinaryReader.h"

GameUnitParameters::GameUnitParameters(MAXUnitConfig            *config,
                                       MAXClanConfig            *clanBonuses,
                                       PlayerResearchManager    *researchManager,
                                       PlayerUpgradeManager     *upgradeManager)
:_wconfig(config), _wclanBonuses(clanBonuses), _wresearchManager(researchManager), _wupgradeManager(upgradeManager)
{}

GameUnitParameters::GameUnitParameters(BinaryReader             *saveReader)
{}

GameUnitParameters::~GameUnitParameters()
{}

void GameUnitParameters::Upgrade()
{}