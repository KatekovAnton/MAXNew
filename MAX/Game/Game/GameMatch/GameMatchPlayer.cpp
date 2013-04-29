//
//  GameМatchPlayer.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatchPlayer.h"
#include "Texture.h"

#include "GameUnit.h"
#include "GameUnitParameters.h"
#include "GameUnitCurrentState.h"
#include "GameMatch.h"

#include "MAXEngine.h"
#include "MAXClanConfig.h"
#include "MAXUnitObject.h"
#include "MAXContentLoader.h"
#include "MAXConfigManager.h"

#include "PlayerUpgradeManager.h"
#include "PlayerResearchManager.h"
#include "PlayerResourceMap.h"
#include "PlayerBase.h"

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
    _resourceMap = new PlayerResourceMap(match->_map->GetMapWidth(), match->_map->GetMapHeight());
}

GameMatchPlayer::~GameMatchPlayer()
{
    delete _upgradeManager;
    delete _researchManager;
    delete _resourceMap;

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

bool GameMatchPlayer::GetIsCurrentPlayer() const
{
    return this == _match_w->_currentPlayer_w;
}

GameUnit* GameMatchPlayer::CreateUnit (int posx, int posy, string type, unsigned int ID)
{
    MAXObjectConfig* unit = MAXConfigManager::SharedMAXConfigManager()->GetConfig(type);
    GameUnitParameters* params = new GameUnitParameters(unit, _clanConfig, _researchManager, _upgradeManager);
    MAXUnitObject *unitObject = MAXSCL->CreateUnit(unit);
    unitObject->_playerId = _playerInfo._playerId;
    unitObject->_playerPalette_w = GetPalettePointer();
    if (unit->_isMine)
    {
        if (_playerInfo._clan >= 0 && _playerInfo._clan <= 7)
        {
            unitObject->headOffset += _playerInfo._clan * 2;
        }
    }
    
    GameUnit* result = new GameUnit(unitObject, params);
    result->SetColor(GLKVector4Make(_playerInfo._color.r, _playerInfo._color.g, _playerInfo._color.b, 1.0));
    result->SetLocation(CCPoint(posx, posy));
    result->CheckBodyAndShadow();
    result->_delegate_w = this;
    result->_owner_w = this;
    
    
    _units.addObject(result);
    return result;
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

bool GameMatchPlayer::CanSeeUnit(GameUnit* unit)
{
    //TODO: add invisibility conditions
    return (_fog->GetValue(unit->GetUnitCell())>0);
}

#pragma mark - GameFogDelegate

bool GameMatchPlayer::UnitShouldUpdateFog(const GameUnit *unit, const GameFog *fog) const
{
    if (fog == _resourceMapFog) 
        return unit->_unitCurrentParameters->_unitBaseParameters->GetIsSurvivor();
    
    if (fog == _fog) 
        return true;
    
    return false;
}

float GameMatchPlayer::UnitScanRadiusForFog(const GameUnit *unit, const GameFog *fog) const
{
    if (fog == _fog) 
        return unit->_unitCurrentParameters->_unitBaseParameters->_pMaxScan;
    
    if (fog == _resourceMapFog) 
        return 1.5;
    
    return 0;
}

void GameMatchPlayer::CellDidUpdate(const int x, const int y, const GameFog *fog, bool visibleFlag) const
{
    FOG_TYPE type = FOG_TYPE_SIMPLE;
    if (fog == _resourceMapFog)
    {
        _resourceMap->AddCell(x, y);
        type = FOG_TYPE_RESOURCES;
    }
    _match_w->CellDidUpdate(x, y, type, visibleFlag, this);

}

#pragma mark - GameUnitDelegate

void GameMatchPlayer::GameUnitWillLeaveCell(GameUnit *unit)
{
    _fog->UpdateOnUnitDidStartMove(unit);
    _resourceMapFog->UpdateOnUnitDidStartMove(unit);
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidEnterCell(GameUnit *unit)
{
    _fog->UpdateOnUnitDidEndMove(unit);
    _resourceMapFog->UpdateOnUnitDidEndMove(unit);
    _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidDestroy(GameUnit *unit)
{
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidPlaceOnMap(GameUnit *unit)
{
    _fog->UpdateOnUnitDidPlaceToMap(unit);
    _resourceMapFog->UpdateOnUnitDidPlaceToMap(unit);
    _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidRemoveFromMap(GameUnit *unit)
{
    _fog->UpdateOnUnitDidRemoveFromMap(unit);
    _resourceMapFog->UpdateOnUnitDidRemoveFromMap(unit);
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
}


