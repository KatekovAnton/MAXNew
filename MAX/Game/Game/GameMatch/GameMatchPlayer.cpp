//
//  GameМatchPlayer.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatchPlayer.h"
#include "Texture.h"
#include "PlayerBase.h"
#include "GameUnit.h"
#include "GameUnitParameters.h"
#include "MAXEngine.h"
#include "Texture.h"
#include "MAXClanConfig.h"
#include "MAXContetnLoader.h"
#include "MAXConfigManager.h"
#include "PlayerUpgradeManager.h"
#include "PlayerResearchManager.h"

GameMatchPlayer::GameMatchPlayer(GameMatchPlayerInfo info, GameMatch *match)
:_match_w(match)
{
    _playerInfo = info;
    _palette = MAXSCL->TexturePalleteFormDefaultPalleteAndPlayerColor(_playerInfo._color);
    
    _researchManager = new PlayerResearchManager(this);
    _upgradeManager = new PlayerUpgradeManager(this);
}

GameMatchPlayer::~GameMatchPlayer()
{
    delete _upgradeManager;
    delete _researchManager;
    
    delete _palette;
    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        unit->RemoveFromMap();
        delete unit;
    }
}

GameUnit* GameMatchPlayer::CreateUnit (int posx, int posy, string type, unsigned int ID)
{
    MAXObjectConfig* unit = MAXConfigManager::SharedMAXConfigManager()->GetConfig(type);
    GameUnitParameters* params = new GameUnitParameters(unit, _clanConfig, _researchManager, _upgradeManager);
    GameUnit* result = new GameUnit(MAXSCL->CreateUnit(unit), params, this);
    result->SetLocation(CCPoint(posx, posy));
    result->CheckBodyAndShadow();
    _units.addObject(result);
    return result;
}

GameUnit* GameMatchPlayer::GetUnitInPosition(const CCPoint& pos)
{
    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        if (unit->IsUnitInCell(pos)) {
            return unit;
        }
    }
    return NULL;
}
