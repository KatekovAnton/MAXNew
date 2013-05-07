//
//  GameMatch.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXGame.h"
#include "GameUnit.h"
#include "GameMatch.h"
#include "GameMatchPlayer.h"
#include "GameMap.h"
#include "MAXContentLoader.h"
#include "MAXConfigManager.h"
#include "MAXEngine.h"
#include "MAXMapObject.h"
#include "MAXMapMaterial.h"
#include "GameMapResources.h"
#include "GameSettings.h"
#include "Pathfinder.h"
#include "MatchMapAgregator.h"

void GameMatch::DebugLandPlayer(GameMatchPlayer* player, const int i)
{
    player->LandingTo(ccp(20 + i * 20 , 50));
    _resources->LandPlayerAt((int)player->_landingPosition.x, (int)player->_landingPosition.y);
}

GameMatch::GameMatch(const string& configName, const string& clanConfigName, const string& mapName, const vector<GameMatchPlayerInfo>& players)
{
    MAXConfigManager::SharedMAXConfigManager()->LoadConfigsFromFile(configName);
    MAXConfigManager::SharedMAXConfigManager()->LoadClanConfigsFromFile(clanConfigName);
    
    _gameSettings = new GameSettings();
    
    shared_ptr<MAXContentMap> map1 = MAXSCL->LoadMapWithName(mapName);
    _map = new GameMap(map1);
    engine->SetMap(_map->_contentMap);
    
    _resources = new GameMapResources(_gameSettings, _map);
    _resources->GenerateInitialResources();
    
    for (int i = 0; i < players.size(); i++)
    {
        GameMatchPlayer* player = new GameMatchPlayer(players[i], this);
        _players.push_back(player);
        
        DebugLandPlayer(player, i); // ToDo: Remove debug code
    }
    
    _currentPlayer_w = _players[0];
    
    _agregator = new MatchMapAgregator(_map);
    _pathfinder = new Pathfinder(_agregator);
}

GameMatch::~GameMatch()
{
    if (_map)
        delete _map;
    if (_resources)
        delete _resources;
    if (_gameSettings)
        delete _gameSettings;
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player = _players[i];
        delete player;
    }
    if (_pathfinder)
        delete _pathfinder;
}

bool GameMatch::EndTurn()
{
    GameMatchPlayer* nextPlayer = _players[0];
    bool found = false;
    for (int i = 0; i < _players.size(); i++)
    {
        if (found)
        {
            nextPlayer = _players[i];
            break;
        }
        else if (_currentPlayer_w == _players[i])
        {
            found = true;
        }
    }
//    _currentPlayer_w = nextPlayer; // not ready for change player
    _currentPlayer_w->BeginTurn();
    return true;
}

void GameMatch::UpdateConnectorsForUnit(GameUnit* unit)
{
    if (!unit->GetIsConnectored())
        return;
    
    unit->UpdateConnectors();
    USimpleContainer<GameUnit*> *neighborUnits = new USimpleContainer<GameUnit*>(10);
    vector<CCPoint> points = unit->GetNerbyCells();
    for (int i = 0; i < points.size(); i++) {
        CCPoint point = points[i];
        _agregator->FindAllConnectoredUnits(point.x, point.y, unit->_owner_w, neighborUnits);
    }
    
    
    for (int i = 0; i < neighborUnits->GetCount(); i++) {
        neighborUnits->objectAtIndex(i)->UpdateConnectors();
    }
    delete neighborUnits;
    
}

void GameMatch::GameUnitWillLeaveCell(GameUnit *unit, const CCPoint &point)
{
    _agregator->RemoveUnitFromCell(unit, point.x, point.y);
    
    
    UpdateConnectorsForUnit(unit);
}

void GameMatch::GameUnitDidEnterCell(GameUnit *unit, const CCPoint &point)
{
    bool needMessage = false;
    if (_currentPlayer_w->CanSeeUnit(unit))
    {
        _agregator->AddUnitToCell(unit, point.x, point.y);
        UpdateConnectorsForUnit(unit);
        needMessage = !unit->_onDraw && unit->_owner_w != _currentPlayer_w;
        unit->Show();
    }
    else
    {
        unit->Hide();
    }
    if (needMessage) {
        game->ShowUnitSpottedMessage(unit);
    }
}

USimpleContainer<GameUnit*> GameMatch::GetAllUnitsInCell(const int x, const int y)
{
    USimpleContainer<GameUnit*> units;
    
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player = _players[i];
        if (player != _currentPlayer_w)
        {
            for (int i = 0; i < player->_units.GetCount(); i++)
            {
                GameUnit* unit = player->_units.objectAtIndex(i);
                CCPoint pos = unit->GetUnitCell();
                if (((int)pos.x == x) && ((int)pos.y == y))
                {
                    units.addObject(unit);
                }
            }
        }
    }
    
    return units;
}

void GameMatch::CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag, const GameMatchPlayer* _player)
{
    if (_player->GetIsCurrentPlayer())
    {
        if (type == FOG_TYPE_SIMPLE)
        {
            engine->AddFogCell(x, y, !visibleFlag);
            USimpleContainer<GameUnit*> units = GetAllUnitsInCell(x, y);
            if (visibleFlag)
            {
                bool needMessage = false;
                for (int i = 0; i < units.GetCount(); i++)
                {
                    GameUnit *unit = units.objectAtIndex(i);
                    needMessage = ! unit->_onDraw &&  unit->_owner_w != _currentPlayer_w;
                    unit->Show();
                    _agregator->AddUnitToCell(unit, x, y);
                    
                    if (needMessage) {
                        game->ShowUnitSpottedMessage(unit);
                    }
                }
            }
            else
            {
                for (int i = 0; i < units.GetCount(); i++)
                {
                    GameUnit *unit = units.objectAtIndex(i);
                    unit->Hide();
                    _agregator->RemoveUnitFromCell(unit, x, y);
                }
            }
        }
        if (type == FOG_TYPE_RESOURCES && visibleFlag)
        {
            engine->AddResourceCell(x, y, _resources->GetResourceTypeAt(x, y), _resources->GetResourceValueAt(x, y));
        }
    }
    else
    {
        //TODO: drop invisibility from our units if need
    }
    
    
    
}

bool GameMatch::GetIsCellValid(CCPoint cell) const
{
    return cell.x>=0 && cell.y>=0 && cell.x <_map->_w && cell.y < _map->_h;
}


