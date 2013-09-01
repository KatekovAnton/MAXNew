//
//  GameMatchPlayerData.cpp
//  MAX
//
//  Created by  Developer on 07.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatchPlayerData.h"
#include "GameMatchPlayerDataDelegate.h"
#include "GameMatch.h"

#include "PlayerUpgradeManager.h"
#include "PlayerResearchManager.h"
#include "PlayerResourceMap.h"
#include "PlayerBase.h"

#include "GameFog.h"
#include "GameMap.h"
#include "GameUnitBaseParameters.h"
#include "GameUnitData.h"

#include "MAXConfigManager.h"
#include "MAXObjectConfig.h"
#include "MAXClanConfig.h"

GameMatchPlayerData::GameMatchPlayerData(GameMatchPlayerInfo playerInfo, GameMatch *match)
:_playerInfo(playerInfo), cameraZoom(1)
{
    
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
    MAXClanConfig *clanConfig = MAXConfigManager::SharedMAXConfigManager()->GetClanConfig(playerInfo._clan);
    for (int i = 0; i < allUnits.size(); i++)
    {
        string type = allUnits[i];
		if (_unitConfigs.count(type) != 0)
		{
 			int a = 0;
			a++;
		}
        MAXObjectConfig *unitConfig = MAXConfigManager::SharedMAXConfigManager()->GetUnitConfig(type);
        GameUnitBaseParameters *params = new GameUnitBaseParameters(unitConfig, clanConfig);
        _unitConfigs.insert(pair<string, GameUnitBaseParameters*>(toLower(type), params));
    }
}

GameMatchPlayerData::~GameMatchPlayerData()
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
        delete fogs[i];
    
    
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

bool GameMatchPlayerData::CanSeeUnit(GameUnitData* unit)
{
    bool result = false;
    if (unit->_ownerId != _playerInfo._playerId)
    {
        for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
        {
            if (UnitCoveredByFog(unit, fogs[i]))
            {
                if (unit->GetSize() == 1)
                {
                    return fogs[i]->GetValue(unit->_unitCell) > 0;
				}
                else
                {
                    CCPoint unitCell = unit->_unitCell;
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

					return false;
                }
            }
        }
    }
    else
        return true;
    return result;
}

bool GameMatchPlayerData::UnitShouldUpdateFog(const GameUnitData *unit, const GameFog *fog) const
{
    if (unit->_isUnderConstruction)
        return false;
    
    bool result = false;
    switch (fog->type)
    {
        case FOG_TYPE_SCAN:
            result = (unit->GetMaxParameterValue(UNIT_PARAMETER_TYPE_SCAN) > 0);//true to allow units with 0 scan see its own cells
            break;
        case FOG_TYPE_RESOURCES:
            result = unit->GetIsSurvivor();
            break;
        case FOG_TYPE_MINES:
            result = unit->GetConfig()->_isSeeMines;
            break;
        case FOG_TYPE_UNDERWATER:
            result = unit->GetConfig()->_isSeeUnderwater;
            break;
        case FOG_TYPE_INFILTRATOR:
            result = unit->GetConfig()->_isAntiStealth;
            break;
            
        case FOG_TYPE_MAX:
            break;
    }
    return result;
}

//unit dont shows with this fog
bool GameMatchPlayerData::UnitCoveredByFog(const GameUnitData *unit, const GameFog *fog) const
{
    bool result = false;
    switch (fog->type)
    {
        case FOG_TYPE_SCAN:
            if (unit->GetIsUnderwater())
            {
                EXTENDED_GROUND_TYPE groundType = _delegate_w->GroudTypeAtPoint(unit->_unitCell.x, unit->_unitCell.y);
                return groundType != EXTENDED_GROUND_TYPE_WATER;
            }
            result = !(unit->GetConfig()->_isStealth || unit->GetConfig()->_isBombMine);
            break;
        case FOG_TYPE_RESOURCES:
            result = false;
            break;
        case FOG_TYPE_MINES:
            result = unit->GetConfig()->_isBombMine;
            break;
        case FOG_TYPE_UNDERWATER:
        {
            MAXObjectConfig* config = unit->GetConfig();
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
                    EXTENDED_GROUND_TYPE groundType = _delegate_w->GroudTypeAtPoint(unit->_unitCell.x, unit->_unitCell.y);
                    if (groundType != GROUND_TYPE_WATER)
                        result = false;
                }
            }
            break;
        }
        case FOG_TYPE_INFILTRATOR:
            result = unit->GetConfig()->_isStealth;
            break;
            
        case FOG_TYPE_MAX:
            break;
    }
    return result;
}

void GameMatchPlayerData::UnitDidStartMove(GameUnitData *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidStartMove(unit);
    }
}

void GameMatchPlayerData::UnitDidEndMove(GameUnitData *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidEndMove(unit);
    }
}

void GameMatchPlayerData::UnitDidRemoveFromMap(GameUnitData *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidRemoveFromMap(unit);
    }
}

void GameMatchPlayerData::UnitDidPlaceToMap(GameUnitData *unit)
{
    for (int i = FOG_TYPE_MIN; i < FOG_TYPE_MAX; i++)
    {
        if (UnitShouldUpdateFog(unit, fogs[i]))
            fogs[i]->UpdateOnUnitDidPlaceToMap(unit);
    }
}

#pragma mark - GameFogDelegate

float GameMatchPlayerData::UnitScanRadiusForFog(const GameUnitData *unit, const GameFog *fog) const
{
    float result = 0;
    switch (fog->type)
    {
        case FOG_TYPE_SCAN:
        case FOG_TYPE_UNDERWATER:
        case FOG_TYPE_INFILTRATOR:
            result = unit->GetMaxParameterValue(UNIT_PARAMETER_TYPE_SCAN);
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

void GameMatchPlayerData::CellDidUpdate(const int x, const int y, const GameFog *fog, bool visibleFlag) const
{
    switch (fog->type)
    {
        case FOG_TYPE_RESOURCES:
            _resourceMap->AddCell(x, y);
            break;
        default:
            break;
    }
    if (_delegate_w)
        _delegate_w->CellDidUpdate(x, y, fog->type, visibleFlag);
}

int GameMatchPlayerData::FogValueInCell(FOG_TYPE fogType, const CCPoint &cell)
{
    return fogs[(int)fogType]->GetValue(cell);
}
