//
//  GameMatch.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXGame.h"
#include "GameUnit.h"
#include "GameFog.h"
#include "GameMatch.h"
#include "GameMatchPlayer.h"
#include "GameMap.h"
#include "GameUnitData.h"
#include "MAXContentLoader.h"
#include "MAXConfigManager.h"
#include "MAXEngine.h"
#include "MAXMapObject.h"
#include "MAXMapMaterial.h"
#include "GameMapResources.h"
#include "GameSettings.h"
#include "Pathfinder.h"
#include "MatchMapAgregator.h"
#include "PlayerResourceMap.h"
#include "PFWaveCell.h"

void GameMatch::DebugLandPlayer(GameMatchPlayer* player, const int i)
{
    player->LandingTo(ccp(20 + i * 20 , 50));
    _resources->LandPlayerAt((int)player->_landingPosition.x, (int)player->_landingPosition.y);
}

GameMatch::GameMatch(const string& configName, const string& clanConfigName, const string& mapName, const vector<GameMatchPlayerInfo>& players)
:_currentTurn(1)
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
    _fullAgregator = new MatchMapAgregator(_map);
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
    _currentPlayer_w->EndTurn();
    _playersCompleteTurn.push_back(_currentPlayer_w);
    
    
    GameMatchPlayer* nextPlayer = _players[0];
    if (_playersCompleteTurn.size() == _players.size())
    {
        _playersCompleteTurn.clear();
    }
    else
    {
        nextPlayer = _players[_playersCompleteTurn.size()];
    }
    
    
    
    
    _currentPlayer_w = nextPlayer;
    
    _agregator->ClearAllData();
    
    //update units
    _currentPlayer_w->BeginTurn();

    for (int i = 0; i < _players.size(); i++)
    {
        if (_currentPlayer_w != _players[i])
        {
            USimpleContainer<GameUnit*> *units = &_players[i]->_units;
            for (int i = 0; i < units->GetCount(); i++)
            {
                GameUnit* unit = units->objectAtIndex(i);
                //GameUnitDidEnterCell(unit, unit->GetUnitCell());
                CCPoint point = unit->GetUnitCell();
                if (_currentPlayer_w->CanSeeUnit(unit))
                {
                    _agregator->AddUnitToCell(unit, point.x, point.y);
                    UpdateConnectorsForUnit(unit);
                    unit->Show();
                }
            }
        }
    }
    
    //update fog
    engine->FillFog();
    UnfillFogOnStartTurn();
    engine->ClearResourceFog();
    FillResourceFogOnStartTurn();
    
    return true;
}

void GameMatch::UnfillFogOnStartTurn()
{
    for (int i = 0; i < _map->_w; i++) {
        for (int j = 0; j < _map->_h; j++) {
            bool see = _currentPlayer_w->fogs[FOG_TYPE_SCAN]->GetValue(ccp(i, j))>0;
            if (see)
                engine->AddFogCell(i, j, false);
        }
    }
}

void GameMatch::FillResourceFogOnStartTurn()
{
    for (int x = 0; x < _map->_w; x++) {
        for (int y = 0; y < _map->_h; y++) {
            bool see = _currentPlayer_w->_resourceMap->GetValue(x, y);
            if (see)
                engine->AddResourceCell(x, y, _resources->GetResourceTypeAt(x, y), _resources->GetResourceValueAt(x, y));
        }
    }
}

void GameMatch::UpdateConnectorsForUnit(GameUnit* unit)
{
    if (!unit->_unitData->GetIsConnectored())
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
    _fullAgregator->RemoveUnitFromCell(unit, point.x, point.y);
    _agregator->RemoveUnitFromCell(unit, point.x, point.y);
    UpdateConnectorsForUnit(unit);
}

void GameMatch::GameUnitDidUndetected(GameUnit *unit, const CCPoint &point)
{
    if (!_currentPlayer_w->CanSeeUnit(unit))
    {
        unit->Hide();
        _agregator->RemoveUnitFromCell(unit, unit->GetUnitCell().x, unit->GetUnitCell().y);
    }
    else if (unit->_owner_w != _currentPlayer_w)
    {
        unit->DetectedByPlayer(_currentPlayer_w->_playerInfo._playerId);
    }
}

void GameMatch::GameUnitDidDetected(GameUnit *unit, const CCPoint &point)
{
    if (_currentPlayer_w->CanSeeUnit(unit))
    {
        unit->Show();
        _agregator->AddUnitToCell(unit, unit->GetUnitCell().x, unit->GetUnitCell().y);
    }
}

void GameMatch::GameUnitDidEnterCell(GameUnit *unit, const CCPoint &point)
{
    _fullAgregator->AddUnitToCell(unit, point.x, point.y);
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
    if (needMessage)
    {
        game->ShowUnitSpottedMessage(unit);
    }
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player = _players[i];
        if (player != unit->_owner_w)
        {
            if (unit->GetIsStealthable() && player->CanSeeUnit(unit))
            {
                if (!unit->IsDetectedByPlayer(player->_playerInfo._playerId))
                {
                    unit->DetectedByPlayer(player->_playerInfo._playerId);
                }
            }
        }
    }
}

vector<GameUnit*> GameMatch::GetAllUnitsInCell(const int x, const int y)
{
    vector<GameUnit*> units;
    
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
                    units.push_back(unit);
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
        if (type == FOG_TYPE_RESOURCES && visibleFlag)
        {
            engine->AddResourceCell(x, y, _resources->GetResourceTypeAt(x, y), _resources->GetResourceValueAt(x, y));
        }
        else
        {
            if (type == FOG_TYPE_SCAN)
            {
                engine->AddFogCell(x, y, !visibleFlag);
            }
            if (x==62 && y == 50 && visibleFlag && type == FOG_TYPE_SCAN) {
                int a = 0;
                a++;
            }
            USimpleContainer<GameUnit*> *units = _fullAgregator->UnitsInCell(x, y);
            if (visibleFlag)
            {
                bool needMessage = false;
                for (int i = 0; i < units->GetCount(); i++)
                {
                    GameUnit *unit = units->objectAtIndex(i);
                    if (_currentPlayer_w->CanSeeUnit(unit))
                    {
                        needMessage = ! unit->_onDraw &&  unit->_owner_w != _currentPlayer_w;
                        if (needMessage)
                        {
                            game->ShowUnitSpottedMessage(unit);
                        }
                        
                        if (type != FOG_TYPE_SCAN)
                        {
                            unit->DetectedByPlayer(_currentPlayer_w->_playerInfo._playerId);
                        }
                        
                        unit->Show();
                        CCPoint p = unit->GetUnitCell();
                        _agregator->AddUnitToCell(unit, p.x, p.y);
                    }
                }
            }
            else
            {
                for (int i = 0; i < units->GetCount(); i++)
                {
                    GameUnit *unit = units->objectAtIndex(i);
                    if (!_currentPlayer_w->CanSeeUnit(unit))
                    {
                        unit->Hide();
                        _agregator->RemoveUnitFromCell(unit, x, y);
                    }
                }
            }
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

bool GameMatch::IsHiddenUnitInPos(const int x, const int y, const bool checkOnly)
{
	bool result = false;
	for (int i = 0; i < _players.size(); i++)
	{
		GameMatchPlayer* player = _players[i];
		for (int i = 0; i < player->_units.GetCount(); i++)
		{
			GameUnit* unit = player->_units.objectAtIndex(i);
			if (unit->GetIsStealthable())
			{
				CCPoint pos = unit->GetUnitCell();
				if (((int)pos.x == x) && ((int)pos.y == y))
				{
					if (checkOnly)
					{
						result = true;
						break;
					}
					else
					{
						// try to quick move if possible to prevent detecting
                        bool escaped = game->EscapeStealthUnitFromPos(unit, x, y);
						if (!escaped)
						{
							result = true;
							game->ShowUnitSpottedMessage(unit);
							unit->DetectedByPlayer(_currentPlayer_w->_playerInfo._playerId);
							unit->Show();
							_agregator->AddUnitToCell(unit, x, y);
							break;
						}
					}					
				}
			}
		}
		if (result)
		{
			break;
		}
	}
	return result;
}
