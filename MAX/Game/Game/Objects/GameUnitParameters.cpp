//
//  GameUnitParameters.cpp
//  MAX
//
//  Created by Anton Katekov on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitParameters.h"
#include "MAXUnitConfig.h"
#include "PlayerResearchManager.h"
#include "PlayerUpgradeManager.h"
#include "PlayerClanBonuses.h"
#include "BinaryReader.h"

GameUnitParameters::GameUnitParameters(MAXUnitConfig            *config,
                                       PlayerClanBonuses        *clanBonuses,
                                       PlayerResearchManager    *researchManager,
                                       PlayerUpgradeManager     *upgradeManager)
{}

GameUnitParameters::GameUnitParameters(BinaryReader             *saveReader)
{}

GameUnitParameters::~GameUnitParameters()
{}