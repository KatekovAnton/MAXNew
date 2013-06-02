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
#include "GameUnitBaseParameters.h"
#include "GameUnitParameters.h"
#include "GameUnitData.h"
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
#include "MatchMapAgregator.h"

GameMatchPlayer::GameMatchPlayer(GameMatchPlayerInfo info, GameMatch *match)
:_match_w(match), cameraZoom(1)
{
    _playerInfo = info;
    _palettes = MAXSCL->TexturePalletesFormDefaultPalleteAndPlayerColor(_playerInfo._color);
    _palette = _palettes[0];
    
    _researchManager = new PlayerResearchManager(this);
    _upgradeManager = new PlayerUpgradeManager(this);
    int w = match->_map->GetMapWidth();
    int h = match->_map->GetMapHeight();
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        fogs[i] = new GameFog(w, h);
        fogs[i]->_delegate_w = this;
        fogs[i]->type = (FOG_TYPE)i;
    }
    
    _resourceMap = new PlayerResourceMap(w, h);
    
    vector<string> allUnits = MAXConfigManager::SharedMAXConfigManager()->GetAllUnits();
    MAXClanConfig *clanConfig = MAXConfigManager::SharedMAXConfigManager()->GetClanConfig(info._clan);
    for (int i = 0; i < allUnits.size(); i++)
    {
        string type = allUnits[i];
        MAXObjectConfig *unitConfig = MAXConfigManager::SharedMAXConfigManager()->GetUnitConfig(type);
        GameUnitBaseParameters *params = new GameUnitBaseParameters(unitConfig, clanConfig);
        _unitConfigs.insert(pair<string, GameUnitBaseParameters*>(toLower(type), params));
    }
}

GameMatchPlayer::~GameMatchPlayer()
{

    for (int i = 0; i < _palettes.size(); i++) {
        Texture* t = _palettes[i];
        delete t;
    }
    
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        delete fogs[i];
    }

    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        unit->RemoveUnitFromMap();
        delete unit;
    }
    
    
    delete _upgradeManager;
    delete _researchManager;
    delete _resourceMap;
    
    
    vector<string> allUnits = MAXConfigManager::SharedMAXConfigManager()->GetAllUnits();
    for (int i = 0; i < allUnits.size(); i++) {
        GameUnitBaseParameters *params = _unitConfigs[allUnits[i]];
        delete params;
    }
    _unitConfigs.clear();
}

bool GameMatchPlayer::GetIsCurrentPlayer() const
{
    return this == _match_w->_currentPlayer_w;
}

GameUnit* GameMatchPlayer::CreateUnit (int posx, int posy, string type, unsigned int ID)
{
    string lowerType = toLower(type);
    GameUnitBaseParameters* unit = _unitConfigs[lowerType];
    GameUnitParameters* params = new GameUnitParameters(unit, _researchManager, _upgradeManager);
    MAXUnitObject *unitObject = MAXSCL->CreateUnit(unit->_configObject);
    unitObject->_playerId = _playerInfo._playerId;
    unitObject->_playerPalette_w = GetPalettePointer();
    if (unit->GetIsMine())
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
    bool result = false;
    if (unit->_owner_w != this)
    {
        for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
        {
            if ((i == FOG_TYPE_SCAN) || (!unit->IsDetectedByPlayer(_playerInfo._playerId)))
            {
                if (UnitCoveredByFog(unit, fogs[i]))
                {
                    CCPoint unitCell = unit->GetUnitCell();
                    if (unit->_unitData->GetSize() == 1)
                    {
                        if (fogs[i]->GetValue(unitCell) > 0)
                            return true;
                    }
                    else
                    {
                        CCPoint unitCell = unit->GetUnitCell();
                        if (fogs[i]->GetValue(unitCell) > 0)
                            return true;
                        unitCell.x += 1;
                        if (fogs[i]->GetValue(unitCell) > 0)
                            return true;
                        unitCell.x -= 1;
                        unitCell.y += 1;
                        if (fogs[i]->GetValue(unitCell) > 0)
                            return true;
                        unitCell.x += 1;
                        if (fogs[i]->GetValue(unitCell) > 0)
                            return true;
                    }
                }
            }
        }
    }
    else
        return true;
    return result;
}

bool GameMatchPlayer::UnitShouldUpdateFog(const GameUnit *unit, const GameFog *fog) const
{
    if (unit->GetIsConstruction()) 
        return false;
    
    bool result = false;
    switch (fog->type)
    {
        case FOG_TYPE_SCAN:
            result = (unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_SCAN) > 0);
            break;
        case FOG_TYPE_RESOURCES:
            result = unit->_unitData->GetIsSurvivor();
            break;
        case FOG_TYPE_MINES:
            result = unit->_unitData->GetConfig()->_isSeeMines;
            break;
        case FOG_TYPE_UNDERWATER:
            result = unit->_unitData->GetConfig()->_isSeeUnderwater;
            break;
        case FOG_TYPE_INFILTRATOR:
            result = unit->_unitData->GetConfig()->_isAntiStealth;
            break;

        case FOG_TYPE_MAX:
            break;
    }
    return result;
}

bool GameMatchPlayer::UnitCoveredByFog(const GameUnit *unit, const GameFog *fog) const
{
    bool result = false;
    switch (fog->type)
    {
        case FOG_TYPE_SCAN:
            result = !(unit->_unitData->GetConfig()->_isStealth || unit->_unitData->GetIsUnderwater() || unit->_unitData->GetConfig()->_isBombMine);
            break;
        case FOG_TYPE_RESOURCES:
            result = false;
            break;
        case FOG_TYPE_MINES:
            result = unit->_unitData->GetConfig()->_isBombMine;
            break;
        case FOG_TYPE_UNDERWATER:
        {
            MAXObjectConfig* config = unit->_unitData->GetConfig();
            if (config->_isShip)
            {
                result = config->_isUnderwater;
            }
            else if (config->_isAmphibious)
            {
                result = config->_isUnderwater;
                if (result)
                {
                    // check if unit in water
                    EXTENDED_GROUND_TYPE groundType = _match_w->_agregator->GroundTypeAtXY(unit->GetUnitCell().x, unit->GetUnitCell().y);
                    if (groundType != GROUND_TYPE_WATER)
                    {
                        result = false;
                    }
                }
            }
            break;
        }
        case FOG_TYPE_INFILTRATOR:
            result = unit->_unitData->GetConfig()->_isStealth;
            break;
            
        case FOG_TYPE_MAX:
            break;
    }
    return result;
}

#pragma mark - GameFogDelegate

float GameMatchPlayer::UnitScanRadiusForFog(const GameUnit *unit, const GameFog *fog) const
{
    float result = 0;
    switch (fog->type)
    {
        case FOG_TYPE_SCAN:
        case FOG_TYPE_UNDERWATER:
        case FOG_TYPE_INFILTRATOR:
            result = unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_SCAN);
            break;
        case FOG_TYPE_RESOURCES:
        case FOG_TYPE_MINES:
            result = 1.5;
            break;
        case FOG_TYPE_MAX:
            break;
    }
    return result;
}

void GameMatchPlayer::CellDidUpdate(const int x, const int y, const GameFog *fog, bool visibleFlag) const
{
    switch (fog->type)
    {
        case FOG_TYPE_RESOURCES:
            _resourceMap->AddCell(x, y);
            break;
        default:
            break;
    }
    _match_w->CellDidUpdate(x, y, fog->type, visibleFlag, this);
}

#pragma mark - GameUnitDelegate

void GameMatchPlayer::GameUnitWillLeaveCell(GameUnit *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidStartMove(unit);
    }
    
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidEnterCell(GameUnit *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidEndMove(unit);
    }
    
    _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidDestroy(GameUnit *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidRemoveFromMap(unit);
    }
    
    _match_w->GameUnitWillLeaveCell(unit, unit->GetUnitCell());
    unit->RemoveUnitFromMap();
    _units.removeObject(unit);
    delete unit;
}

void GameMatchPlayer::GameUnitDidPlaceOnMap(GameUnit *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidPlaceToMap(unit);
    }
    
    _match_w->GameUnitDidEnterCell(unit, unit->GetUnitCell());
}

void GameMatchPlayer::GameUnitDidRemoveFromMap(GameUnit *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidRemoveFromMap(unit);
    }
    
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
