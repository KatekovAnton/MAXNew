//
//  GameМatchPlayer.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatchPlayer.h"
#include "GameMatchPlayerData.h"
#include "Texture.h"

#include "GameUnit.h"
#include "GameUnitBaseParameters.h"
#include "GameUnitParameters.h"
#include "GameUnitData.h"
#include "GameMatch.h"

#include "MAXEngine.h"
#include "MAXUnitObject.h"
#include "MAXContentLoader.h"

#include "Pathfinder.h"
#include "PFWaveCell.h"
#include "MatchMapAgregator.h"

#include "StringUtils.h"

GameMatchPlayer::GameMatchPlayer(GameMatchPlayerInfo info, GameMatch *match)
:_match_w(match)
{
    _playerData = new GameMatchPlayerData(info, match);
    _playerData->_delegate_w = this;
    _palettes = MAXSCL->TexturePalletesFormDefaultPalleteAndPlayerColor(info._color);
    _palette = _palettes[0];
    
    
    _agregator = new MatchMapAgregator(match->_map);
    _pathfinder = new Pathfinder(_agregator);
}

GameMatchPlayer::~GameMatchPlayer()
{
    for (int i = 0; i < _palettes.size(); i++) {
        Texture* t = _palettes[i];
        delete t;
    }

    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        unit->RemoveUnitFromMap();
        delete unit;
    }
    
    delete _agregator;
    
    delete _playerData;
    if (_pathfinder)
        delete _pathfinder;
}

bool GameMatchPlayer::GetIsCurrentPlayer() const
{
    return this == _match_w->_currentPlayer_w;
}

int GameMatchPlayer::GetPlayerId() const
{
    return _playerData->_playerInfo._playerId;
}

GameUnit* GameMatchPlayer::CreateUnit (int posx, int posy, string type, unsigned int ID)
{
    string lowerType = toLower(type);
    GameUnitBaseParameters* unit = _playerData->_unitConfigs[lowerType];
    GameUnitParameters* params = new GameUnitParameters(unit, _playerData->_researchManager, _playerData->_upgradeManager);
    MAXUnitObject *unitObject = MAXSCL->CreateUnit(unit->_configObject);
    unitObject->_playerId = _playerData->_playerInfo._playerId;
    unitObject->_playerPalette_w = GetPalettePointer();
    if (unit->GetIsMine())
    {
        if (_playerData->_playerInfo._clan >= 0 && _playerData->_playerInfo._clan <= 7)
            unitObject->headOffset += _playerData->_playerInfo._clan * 2;
    }
    
    GameUnit* result = new GameUnit(unitObject, params, GetPlayerId(), this);
    result->SetColor(GLKVector4Make(_playerData->_playerInfo._color.r, _playerData->_playerInfo._color.g, _playerData->_playerInfo._color.b, 1.0));
    result->SetLocation(CCPoint(posx, posy));
    result->CheckBodyAndShadow();
    result->_delegate_w = this;
    
    
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
    _playerData->_landingPosition = landingPosition;
}

void GameMatchPlayer::BeginTurn()
{
    for (int i = 0; i < _units.GetCount(); i++)
    {
        GameUnit* unit = _units.objectAtIndex(i);
        unit->NewTurn();
        _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
    }
}

void GameMatchPlayer::EndTurn()
{

}

bool GameMatchPlayer::CanSeeUnit(GameUnit* unit)
{
    return _playerData->CanSeeUnit(unit->_unitData);
}

#pragma mark - GameMatchPlayerDataDelegate

EXTENDED_GROUND_TYPE GameMatchPlayer::GroudTypeAtPoint(const int x, const int y)
{
    return _agregator->GroundTypeAtXY(x, y);
}

void GameMatchPlayer::CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag)
{
    _match_w->CellDidUpdate(x, y, type, visibleFlag, this);
}

#pragma mark - GameUnitDelegate

void GameMatchPlayer::GameUnitWillLeaveCell(GameUnit *unit)
{
    _playerData->UnitDidStartMove(unit->_unitData);
    
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidEnterCell(GameUnit *unit)
{
    _playerData->UnitDidEndMove(unit->_unitData);
    
    _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidDestroy(GameUnit *unit)
{
    
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
    _units.removeObject(unit);

    _match_w->GameUnitDidDestroy(unit);
}

void GameMatchPlayer::GameUnitDidPlaceOnMap(GameUnit *unit)
{
    _playerData->UnitDidPlaceToMap(unit->_unitData);
    
    _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidRemoveFromMap(GameUnit *unit)
{
    _playerData->UnitDidRemoveFromMap(unit->_unitData);
    
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidUndetected(GameUnit *unit)
{
    _match_w->GameUnitDidUndetected(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidDetected(GameUnit *unit)
{
    _match_w->GameUnitDidDetected(unit, unit->GetUnitCell());
}
