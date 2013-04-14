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

GameMatch::GameMatch(const string& configName, const string& mapName, const vector<GameMatchPlayerInfo>& players)
{
    MAXConfigManager::SharedMAXConfigManager()->LoadConfigsFromFile(configName);
    
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

void GameMatch::GameUnitWillLeaveCell(GameUnit *unit, const CCPoint &point)
{
    _agregator->RemoveUnitFromCell(unit, point.x, point.y);
}

void GameMatch::GameUnitDidEnterCell(GameUnit *unit, const CCPoint &point)
{
    _agregator->AddUnitToCell(unit, point.x, point.y);
    bool needMessage = false;
    if (_currentPlayer_w->CanSeeUnit(unit))
    {
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

void GameMatch::CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag, const GameMatchPlayer* _player)
{
    if (_player->GetIsCurrentPlayer())
    {
        if (type == FOG_TYPE_SIMPLE)
        {
            engine->AddFogCell(x, y, !visibleFlag);
            USimpleContainer<GameUnit*> *units = _agregator->UnitsInCell(x, y);
            if (visibleFlag)
            {
                bool needMessage = false;
                for (int i = 0; i < units->GetCount(); i++)
                {
                    needMessage = ! units->objectAtIndex(i)->_onDraw &&  units->objectAtIndex(i)->_owner_w != _currentPlayer_w;
                    units->objectAtIndex(i)->Show();
                    
                    if (needMessage) {
                        game->ShowUnitSpottedMessage(units->objectAtIndex(i));
                    }
                }
            }
            else
            {
                for (int i = 0; i < units->GetCount(); i++)
                    units->objectAtIndex(i)->Hide();
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

