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
#include "GameMatch.h"
#include "MAXEngine.h"
#include "Texture.h"
#include "MAXClanConfig.h"
#include "MAXContentLoader.h"
#include "MAXConfigManager.h"
#include "PlayerUpgradeManager.h"
#include "PlayerResearchManager.h"
#include "GameMap.h"
#include "GameFog.h"

GameMatchPlayer::GameMatchPlayer(GameMatchPlayerInfo info, GameMatch *match)
:_match_w(match)
{
    _playerInfo = info;
    _palettes = MAXSCL->TexturePalletesFormDefaultPalleteAndPlayerColor(_playerInfo._color);
    _palette = _palettes[0];
    
    _researchManager = new PlayerResearchManager(this);
    _upgradeManager = new PlayerUpgradeManager(this);
    _fog = new GameFog(match->_map->GetMapWidth(), match->_map->GetMapHeight());
}

GameMatchPlayer::~GameMatchPlayer()
{
    delete _upgradeManager;
    delete _researchManager;

    for (int i = 0; i < _palettes.size(); i++) {
        Texture* t = _palettes[i];
        delete t;
    }
    
    delete _fog;

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
    result->SetColor(GLKVector4Make(_playerInfo._color.r, _playerInfo._color.g, _playerInfo._color.b, 1.0));
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

void GameMatchPlayer::SetPalette(double time)
{
    int tt = ((int)(time*6)) % _palettes.size();
    _palette = _palettes[tt];
}

void GameMatchPlayer::UpdateFogForUnit(GameUnit *unit, const CCPoint &unitPosition)
{
    _fog->Update(unit, unitPosition);
}

void GameMatchPlayer::ResetFogForUnit(GameUnit *unit, const CCPoint &unitPosition)
{
    _fog->Reset(unit, unitPosition);
}
