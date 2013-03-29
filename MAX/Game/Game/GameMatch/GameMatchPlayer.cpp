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
    _fog->_delegate_w = this;
    _resourceMapFog = new GameFog(match->_map->GetMapWidth(), match->_map->GetMapHeight());
    _resourceMapFog->_delegate_w = this;
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
    delete _resourceMapFog;

    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        unit->Hide();
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

void GameMatchPlayer::LandingTo(const CCPoint &landingPosition)
{
    _landingPosition = landingPosition;
}

#pragma mark - GameUnitDelegate

void GameMatchPlayer::UnitDidMove(GameUnit *unit, const CCPoint &oldPoint, const CCPoint &newPoint)
{
    _fog->UpdateOnUnitDidMove(unit, oldPoint, newPoint);
    _resourceMapFog->UpdateOnUnitDidMove(unit, oldPoint, newPoint);
}

void GameMatchPlayer::UnitDidPlaceToMap(GameUnit *unit)
{
    _fog->UpdateOnUnitDidPlaceToMap(unit);
    _resourceMapFog->UpdateOnUnitDidPlaceToMap(unit);
}

void GameMatchPlayer::UnitDidRemoveFromMap(GameUnit *unit)
{
    _fog->UpdateOnUnitDidRemoveFromMap(unit);
    _resourceMapFog->UpdateOnUnitDidRemoveFromMap(unit);
}

#pragma mark - GameFogDelegate

bool GameMatchPlayer::UnitShouldUpdateFog(const GameUnit *unit, const GameFog *fog) const
{
    if (fog == _resourceMapFog) 
        return unit->_config->GetIsSurvivor();
    
    if (fog == _fog) 
        return true;
    
    return false;
}

void GameMatchPlayer::CellDidUpdate(const int cellX, const int cellY, const GameFog *fog, bool visibleFlag) const
{
    if (fog == _fog) {
        
    }
    if (fog == _resourceMapFog) {
        
    }
}



