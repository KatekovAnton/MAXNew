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
#include "GameMatchPlayerData.h"

class GameMap;
class GameMapResources;
class GameSettings;
class GameMatchPlayer;
class MatchMapAgregator;
class MatchFireAggregator;
class Pathfinder;
class MAXObjectConfig;
class MAXGameController;

class GameMatch {

    void DebugLandPlayer(GameMatchPlayer* player, const int i);
    
    
    
public:

	MAXGameController *_gameController;

    
    GameSettings*               _gameSettings;
    GameMap*                    _map;
    GameMapResources*           _resources;
    vector<GameMatchPlayer*>    _players;
    vector<GameMatchPlayer*>    _playersCompleteTurn;
    vector<vector<GameMatchPlayer*>> _teams;
    GameMatchPlayer*            _currentPlayer_w;
    MatchMapAgregator*          _fullAgregator;
    MatchFireAggregator*        _fireAgregator;
    
    int _currentTurn;
    bool _holdAutofire;

    GameMatch(const string& mapName, const vector<GameMatchPlayerInfo>& players);
    ~GameMatch();
    
    bool EndTurn();
    void UnfillFogOnStartTurn();
    void FillResourceFogOnStartTurn();
    
    GameUnit *UnitForAttackingByUnit(GameUnit *agressor, const CCPoint &target);
    bool UnitCanAttackUnit(GameUnit *agressor, GameUnit *target);
    bool UnitCanInteractWithUnit(GameUnit *activeUnit, GameUnit *passiveUnit);
    
    bool PlayerIsEnemyToPlayer(GameMatchPlayer *one, GameMatchPlayer *two);
    
    bool GetCanConstructBuildingInCell(const CCPoint &cell, MAXObjectConfig *buildingType, GameUnit *constructor);
    
    bool GetIsCurrentPlayer(const unsigned int playerId) const { return _currentPlayer_w != NULL && _currentPlayer_w->_playerData->_playerInfo._playerId == playerId; }
    
    void GameUnitWillLeaveCell(GameUnit *unit, const CCPoint &point);
    void GameUnitDidEnterCell(GameUnit *unit, const CCPoint &point);
    
    
    void CheckAutofire(GameUnit *unit, const CCPoint &point);
    
    void CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag, GameMatchPlayer* player);
    bool GetIsCellValid(CCPoint cell) const;
    void UpdateConnectorsForUnit(GameUnit* unit);
	bool IsHiddenUnitInPos(const int x, const int y, const bool checkOnly, GameMatchPlayer *player, vector<CCPoint> lockedCells);
    bool UnitCanBePlacedToCell(const int x, const int y, MAXObjectConfig *buildingType, GameMatchPlayer* player);
    bool UnitCanStillBePlacedToCell(const int x, const int y, MAXObjectConfig *buildingType, GameMatchPlayer* player, bool alreadyPlaced);

    void GameUnitDidDestroy(GameUnit *unit);
    void GameUnitDidInitiateFire(GameUnit *unit);
};

#endif /* defined(__MAX__Game_atch__) */
