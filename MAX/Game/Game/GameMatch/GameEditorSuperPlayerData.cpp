//
//  GameEditorSuperPlayerData.cpp
//  MAX
//
//  Created by  Developer on 24.08.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameEditorSuperPlayerData.h"
#include "GameFog.h"

GameEditorSuperPlayerData::GameEditorSuperPlayerData(GameMatchPlayerInfo playerInfo, GameMatch *match)
:GameMatchPlayerData(playerInfo, match)
{

}

void GameEditorSuperPlayerData::Initialize()
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        fogs[i]->IncreaseAll();
    }
}

bool GameEditorSuperPlayerData::CanSeeUnit(GameUnitData* unit)
{
    return true;
}
