//
//  GameМatch.h
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Game_atch__
#define __MAX__Game_atch__

#include <iostream>
#include "miniPrefix.h"
#include "GameMatchPlayer.h"

class GameMap;
class GameMapResources;
class GameSettings;
class GameMatchPlayer;
class MatchMapAgregator;
class Pathfinder;
class MAXObjectConfig;

class GameMatch {

    void DebugLandPlayer(GameMatchPlayer* player, const int i);
    
public:
    
    GameSettings*               _gameSettings;
    GameMap*                    _map;
    GameMapResources*           _resources;
    Pathfinder*                 _pathfinder;
    vector<GameMatchPlayer*>    _players;
    vector<GameMatchPlayer*>    _playersCompleteTurn;
    GameMatchPlayer*            _currentPlayer_w;
    MatchMapAgregator*          _agregator;
    MatchMapAgregator*          _fullAgregator;
    
    int _currentTurn;

    GameMatch(const string& configName, const string& clanConfigName, const string& mapName, const vector<GameMatchPlayerInfo>& players);
    ~GameMatch();
    
    bool EndTurn();
    void UnfillFogOnStartTurn();
    void FillResourceFogOnStartTurn();
    
    bool GetCanConstructLargeBuildingInCell(const CCPoint &cell, MAXObjectConfig *buildingType);
    
    bool GetIsCurrentPlayer(const unsigned int playerId) const { return _currentPlayer_w != NULL && _currentPlayer_w->_playerInfo._playerId == playerId; }
    
    void GameUnitWillLeaveCell(GameUnit *unit, const CCPoint &point);
    void GameUnitDidUndetected(GameUnit *unit, const CCPoint &point);
    void GameUnitDidDetected(GameUnit *unit, const CCPoint &point);
    void GameUnitDidEnterCell(GameUnit *unit, const CCPoint &point);
    void CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag, const GameMatchPlayer* _player);
    
    bool GetIsCellValid(CCPoint cell) const;
    void UpdateConnectorsForUnit(GameUnit* unit);
	bool IsHiddenUnitInPos(const int x, const int y, const bool checkOnly);
};

#endif /* defined(__MAX__Game_atch__) */
