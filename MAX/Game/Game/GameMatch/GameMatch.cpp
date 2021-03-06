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
#include "GameObject.h"
#include "GameEffect.h"
#include "MAXGameController.h"
#include "MAXContentLoader.h"
#include "MAXConfigManager.h"
#include "MAXEngine.h"
#include "MAXObject.h"
#include "MAXUnitObject.h"
#include "MAXMapObject.h"
#include "MAXMapMaterial.h"
#include "GameMapResources.h"
#include "GameSettings.h"
#include "Pathfinder.h"
#include "MatchMapAgregator.h"
#include "MatchFireAggregator.h"
#include "PlayerResourceMap.h"
#include "PFWaveCell.h"
#include "MAXAnimationPrefix.h"
#include "SoundEngine.h"
#include "MatchGarbageMap.h"

void GameMatch::DebugLandPlayer(GameMatchPlayer* player, const int i)
{
    player->LandingTo(ccp(20 + i * 20 , 50));
    _resources->LandPlayerAt((int)player->_playerData->_landingPosition.x, (int)player->_playerData->_landingPosition.y);
}

GameMatch::GameMatch(const string& mapName, const vector<GameMatchPlayerInfo>& players)
:_currentTurn(1), _holdAutofire(false)
{
    
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
    
    _fullAgregator = new MatchMapAgregator(_map);
    _fireAgregator = new MatchFireAggregator(_map, this);
    _garbageMap = new MatchGarbageMap(_map, _fullAgregator);
}

GameMatch::~GameMatch()
{
    if (_gameSettings)
        delete _gameSettings;
    if (_map)
        delete _map;
	engine->SetMap(nullptr);
    if (_resources)
        delete _resources;
	_gameController->DeletionProgressDidChange(0.1);

    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player_ = _players[i];
        delete player_;
		_gameController->DeletionProgressDidChange((float) i / ((float)_players.size() - 1.0) * 0.7 + 0.1);
    }
    delete _garbageMap;
    delete _fullAgregator;
	_gameController->DeletionProgressDidChange(0.9);
    delete _fireAgregator;
	_gameController->DeletionProgressDidChange(1.0);
    
    
}

void GameMatch::CreateGarbage(const int x, const int y, int size)
{
    _garbageMap->AddGarbageToCell(x, y, size, 10);
    
}

void GameMatch::DestroyGarbage(GarbageElement *garbage)
{}

bool GameMatch::EndTurn()
{
	_holdAutofire = true;
    _currentPlayer_w->EndTurn();
    _playersCompleteTurn.push_back(_currentPlayer_w);
    
    
    GameMatchPlayer* nextPlayer = _players[0];
    if (_playersCompleteTurn.size() == _players.size())
    {
        _playersCompleteTurn.clear();
        _currentTurn ++;
    }
    else
        nextPlayer = _players[_playersCompleteTurn.size()];
    
    _currentPlayer_w = nextPlayer;
    //update units
    _currentPlayer_w->BeginTurn();

    for (int i = 0; i < _players.size(); i++)
    {
        USimpleContainer<GameUnit*> *units = &_players[i]->_units;
        for (int i = 0; i < units->GetCount(); i++)
        {
            GameUnit* unit = units->objectAtIndex(i);
			bool cansee = _currentPlayer_w->CanSeeUnit(unit);
			bool detected = unit->IsDetectedByPlayer(_currentPlayer_w->GetPlayerId()); 
			if ((cansee || detected) && unit->_unitData->_isPlacedOnMap)
                unit->Show();
            else
                unit->Hide();
        }
        
    }
    
    //update fog
    engine->FillFog();
    UnfillFogOnStartTurn();
    engine->ClearResourceFog();
    FillResourceFogOnStartTurn();
    
	_holdAutofire = false;
    return true;
}

void GameMatch::UnfillFogOnStartTurn()
{
    for (int i = 0; i < _map->_w; i++) {
        for (int j = 0; j < _map->_h; j++) {
            bool see = _currentPlayer_w->_playerData->fogs[FOG_TYPE_SCAN]->GetValue(ccp(i, j))>0;
            if (see)
                engine->AddFogCell(i, j, false);
        }
    }
}

void GameMatch::FillResourceFogOnStartTurn()
{
    for (int x = 0; x < _map->_w; x++) {
        for (int y = 0; y < _map->_h; y++) {
            bool see = _currentPlayer_w->_playerData->_resourceMap->GetValue(x, y);
            if (see)
                engine->AddResourceCell(x, y, _resources->GetResourceTypeAt(x, y), _resources->GetResourceValueAt(x, y));
        }
    }
}

GameUnit *GameMatch::UnitForAttackingByUnit(GameUnit *agressor, const CCPoint &target)
{
    USimpleContainer<GameUnit*> *units = _fullAgregator->UnitsInCell(target.x, target.y);
    if (units->GetCount() == 0) 
        return NULL;
    
    GameUnit *result = NULL;
    for (int i = 0; i < units->GetCount(); i++) {
        GameUnit *comparedUnit = units->objectAtIndex(i);
        if (comparedUnit == agressor) 
            continue;
        
        if (UnitCanAttackUnit(agressor, comparedUnit)) {
            if (!result) {
                result = comparedUnit;
                continue;
            }
            if (result->GetConfig()->_bLevel < comparedUnit->GetConfig()->_bLevel && !comparedUnit->GetConfig()->_isBridge && !comparedUnit->GetConfig()->_isRoad && !comparedUnit->GetConfig()->_isPlatform)
                result = comparedUnit;
            
        }
    }
    
    return result;
}

bool GameMatch::UnitCanAttackUnit(GameUnit *agressor, GameUnit *target)
{
	if (target->_unitData->_isUnderConstruction || target->_unitData->GetUniteractable()) 
        return false;
    
    CCPoint targetCell = target->GetUnitCell();
    UNIT_MOVETYPE tmt = (UNIT_MOVETYPE)target->GetConfig()->_bMoveType;
    MAXObjectConfig* agressorConfig = agressor->GetConfig();

    
    switch (tmt) {
        case UNIT_MOVETYPE_GROUND:
        case UNIT_MOVETYPE_GROUNDCOAST:
        case UNIT_MOVETYPE_SEACOAST:
        case UNIT_MOVETYPE_SURVEYOR:
        {
            GROUND_TYPE t = _map->GroundTypeAtXY(targetCell.x, targetCell.y);
            if (t == GROUND_TYPE_WATER || t == GROUND_TYPE_COAST)
                return agressorConfig->_pFireType == 2 || agressorConfig->_pFireType == 1 || agressorConfig->_pFireType == 4;

            return agressorConfig->_pFireType == 1 || agressorConfig->_pFireType == 6 || agressorConfig->_pFireType == 4;
        } break;
            
        case UNIT_MOVETYPE_SEA:
        {
            if (target->GetConfig()->_isUnderwater) {//submarine
                GROUND_TYPE t = _map->GroundTypeAtXY(targetCell.x, targetCell.y);
                if (t == GROUND_TYPE_WATER ) {
                    if (agressorConfig->_bMoveType == UNIT_MOVETYPE_AIR) 
                        return (agressorConfig->_pFireType == 4 || agressorConfig->_pFireType == 1);
                    if (agressorConfig->_bMoveType == UNIT_MOVETYPE_SEA)
                        return agressorConfig->_pFireType == 2 || agressorConfig->_isBombMine;
                    
                    return false;
                }
            }
            return agressorConfig->_pFireType == 2 || agressorConfig->_pFireType == 1 || agressorConfig->_pFireType == 6 || agressorConfig->_pFireType == 4;
        } break;
            
        case UNIT_MOVETYPE_AMHIB:
        {
            if (target->GetConfig()->_isUnderwater) {//pcan
                EXTENDED_GROUND_TYPE t = _fullAgregator->GroundTypeAtXY(targetCell.x, targetCell.y);
                if (t == EXTENDED_GROUND_TYPE_WATER ) {
                    if (agressor->GetConfig()->_bMoveType == UNIT_MOVETYPE_AIR)
                        return (agressor->GetConfig()->_pFireType == 6 || agressor->GetConfig()->_pFireType == 1 || agressorConfig->_pFireType == 4);
                    if (agressor->GetConfig()->_bMoveType == UNIT_MOVETYPE_SEA)
                        return agressor->GetConfig()->_pFireType == 2 || agressorConfig->_pFireType == 4;
                    
                    return false;
                }
            }
            return agressorConfig->_pFireType == 2 || agressorConfig->_pFireType == 1 || agressorConfig->_pFireType == 6 || agressorConfig->_pFireType == 4;
        } break;
            
        case UNIT_MOVETYPE_AIR:
        {
            return agressorConfig->_pFireType == 3 || agressorConfig->_pFireType == 6 || agressorConfig->_pFireType == 4;
        } break;
            
        default:
            break;
    }
    return false;
}

bool GameMatch::UnitCanInteractWithUnit(GameUnit *activeUnit, GameUnit *passiveUnit)
{
	if (passiveUnit->_unitData->_isUnderConstruction || passiveUnit->_unitData->GetUniteractable())
        return false;
    
    CCPoint targetCell = passiveUnit->GetUnitCell();
    UNIT_MOVETYPE pmt = (UNIT_MOVETYPE)passiveUnit->GetConfig()->_bMoveType;
    MAXObjectConfig* activeConfig = activeUnit->GetConfig();
    
    switch (pmt) {
        case UNIT_MOVETYPE_GROUND:
        case UNIT_MOVETYPE_GROUNDCOAST:
        case UNIT_MOVETYPE_SEACOAST:
        case UNIT_MOVETYPE_SURVEYOR:
        case UNIT_MOVETYPE_SEA:
        case UNIT_MOVETYPE_AMHIB:
        {
            return activeConfig->_bMoveType != UNIT_MOVETYPE_AIR || activeUnit->_unitData->_landed;
        } break;
            
        case UNIT_MOVETYPE_AIR:
        {
            if (activeConfig->_bMoveType == UNIT_MOVETYPE_AIR)
                return !activeUnit->_unitData->_landed;
            else
                return passiveUnit->_unitData->_landed;
        } break;
            
        default:
            break;
    }
    return false;
}

bool GameMatch::PlayerIsEnemyToPlayer(GameMatchPlayer *one, GameMatchPlayer *two)
{
    return one != two;
}

void GameMatch::UpdateConnectorsForUnit(GameUnit* unit)
{
    if (!unit->_unitData->GetIsConnectored())
        return;
	if (unit->_unitData->_isUnderConstruction || unit->_unitData->GetUniteractable()) 
        return;
    
    unit->UpdateConnectors();
    USimpleContainer<GameUnit*> *neighborUnits = new USimpleContainer<GameUnit*>(10);
    vector<CCPoint> points = unit->GetNerbyCells();
    for (int i = 0; i < points.size(); i++) {
        CCPoint point = points[i];
        _fullAgregator->FindAllConnectoredUnits(point.x, point.y, unit->_owner_w, neighborUnits);
    }
    
    for (int i = 0; i < neighborUnits->GetCount(); i++) 
        neighborUnits->objectAtIndex(i)->UpdateConnectors();

    delete neighborUnits;
    
}

void GameMatch::GameUnitDidDestroy(GameUnit *unit)
{
    if (_currentPlayer_w->_playerData->fogs[FOG_TYPE_SCAN]->GetValue(unit->GetUnitCell()) > 0 && !unit->_unitData->_isUnderConstruction)
    {
        GROUND_TYPE type = _map->GroundTypeAtPoint(unit->GetUnitCell());
        BLAST_TYPE blastType = BLAST_TYPE_NONE;
        MAXObjectConfig* config = unit->GetConfig();
        EXPLODE_SOUND_TYPE sound = EXPLODE_SOUND_TYPE_NONE;
        if (unit->_unitData->GetIsBuilding())
        {
            if (config->_isMine) 
                sound = EXPLODE_SOUND_TYPE_MINE;
            else if (type == GROUND_TYPE_WATER)
            {
                if (config->_isUnderwater)
                    sound = EXPLODE_SOUND_TYPE_UNDERWATER_UNIT;
                else
                {
                    if (config->_bSize == 1)
                        sound = EXPLODE_SOUND_TYPE_SEA_SMALL_BUILD;
                    else
                        sound = EXPLODE_SOUND_TYPE_SEA_LARGE_BUILD;
                }
            }
            else
            {
                if (config->_bSize == 1) 
                    sound = EXPLODE_SOUND_TYPE_LAND_SMALL_BUILD;
                else
                    sound = EXPLODE_SOUND_TYPE_LAND_LARGE_BUILD;
            }
        }
        else
        {
            if (config->_bMoveType == UNIT_MOVETYPE_AIR)
                sound = EXPLODE_SOUND_TYPE_AIR;
            else
            {
                if (config->_isInfantry)
                    sound = EXPLODE_SOUND_TYPE_INFANTRY;
                else  if (config->_isInfiltrator)
                    sound = EXPLODE_SOUND_TYPE_INFILTRATOR;
                else if (type == GROUND_TYPE_WATER)
                    sound = EXPLODE_SOUND_TYPE_SEA_SMALL_UNIT;
                else
                    sound = EXPLODE_SOUND_TYPE_LAND_SMALL_UNIT;
            }
        }
        
        double delay = 1;
        switch (sound)
        {
            case EXPLODE_SOUND_TYPE_LAND_SMALL_UNIT://explmed.wav explsmal.wav
            case EXPLODE_SOUND_TYPE_LAND_SMALL_BUILD://bldexplg.wav expllrge.wav
                blastType = BLAST_TYPE_GROUND;
                delay = 0.5;
                break;
                
            case EXPLODE_SOUND_TYPE_SEA_SMALL_UNIT://boatexp1.wav
            case EXPLODE_SOUND_TYPE_SEA_SMALL_BUILD://eplowet1.wav eplowet2.wav
            case EXPLODE_SOUND_TYPE_UNDERWATER_UNIT://sub14.wav sub16.wav
                blastType = BLAST_TYPE_SEA;
                delay = 0.5;
                break;
                
            case EXPLODE_SOUND_TYPE_LAND_LARGE_BUILD://expllrge.wav  expbuld5.wav expbuld6.wav explbld1.wav explbld2.wav
            case EXPLODE_SOUND_TYPE_SEA_LARGE_BUILD://cbldexp1.wav cbldexp2.wav
            case EXPLODE_SOUND_TYPE_MINE://cmine16.wav
                blastType = BLAST_TYPE_BUILDING;
                delay = 1;
                break;
                
            case EXPLODE_SOUND_TYPE_AIR://explsmal.wav
                blastType = BLAST_TYPE_AIR;
                delay = 0.5;
                break;
                
            default:
                delay = 0.01;
                break;
        }
        if (unit->GetIsIdleDestroy())
            delay = 0.01;
        
        if (blastType != BLAST_TYPE_NONE && !unit->GetIsIdleDestroy())
        {
            GameEffect* blast = GameEffect::CreateBlast(blastType, config->_bLevel + 1);
            blast->SetLocation(unit->GetUnitCell());
            blast->Show();
            
            MAXAnimationWait* wait = new MAXAnimationWait(blast->GetFrameCount() * 0.1);
            wait->_delegate = blast;
            MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
            
            game->FlushEffectsWithNew(blast);
        }
        if (sound != EXPLODE_SOUND_TYPE_NONE && !unit->GetIsIdleDestroy()) 
            SOUND->PlayExplodeSound(sound);
        
        unit->RemoveWithDelay(delay);
    }
    
    //remove bridge and waterplatform on this cell
    {
        if (!unit->_unitData->GetIsBuilding())
        {
            bool founded = true;
            while (founded)
            {
                founded = false;
                USimpleContainer<GameUnit*> *units = _fullAgregator->UnitsInCell(unit->GetUnitCell().x, unit->GetUnitCell().y);
                for (int i = 0; i < units->GetCount(); i++)
                {
                    GameUnit *cUnit = units->objectAtIndex(i);
                    if (cUnit->GetConfig()->_isBridge || cUnit->GetConfig()->_isPlatform || cUnit->GetConfig()->_isRoad) {
                        cUnit->Destroy(true);
                        founded = true;
                        break;
                    }
                }
            }
        }
    }
}

void GameMatch::GameUnitDidInitiateFire(GameUnit *unit)
{
    MAXObjectConfig* config = unit->GetConfig();
    GameUnitData *_unitData = unit->_unitData;
    CCPoint cell = unit->GetUnitCell();
    if (config->_isStealthable)
    {
        for (int i = 0; i < _players.size(); i++)
        {
            GameMatchPlayer* player = _players[i];
            if (!_unitData->_detected[i] && player->_playerData->FogValueInCell(FOG_TYPE_SCAN, unit->GetUnitCell())>0)
            {
                _unitData->_detected[i] = true;
                player->_agregator->AddUnitToCell(unit, cell.x, cell.y);
                unit->GetUnitObject()->StealthDeactivated();
            }
        }
        CheckAutofire(unit, unit->GetUnitCell());
    }
}

void GameMatch::GameUnitWillLeaveCell(GameUnit *unit, const CCPoint &point)
{
    _fullAgregator->RemoveUnitFromCell(unit, point.x, point.y);
    _fireAgregator->UnitWillLeaveCell(unit, point);
    for (int i = 0; i < _players.size(); i++) {
        _players[i]->_agregator->RemoveUnitFromCell(unit, point.x, point.y);
    }
	UpdateConnectorsForUnit(unit);
	if (unit->GetConfig()->_isPlatform || unit->GetConfig()->_isBridge)
	{
		//if platform or bridge - check if we cant build smth or units cannot be placed 
		//there and should be killed 
		USimpleContainer<GameUnit*> *units_ = _fullAgregator->UnitsInCell(point.x, point.y);
		vector<GameUnit*> units;
		for (int i = 0; i < units_->GetCount(); i++)
			units.push_back(units_->objectAtIndex(i));
		
		for (int i = 0; i < units.size(); i++)
		{
			GameUnit *cunit = units[i];
			CCPoint cell = cunit->GetUnitCell();
			//if we construct smth in this cell-abort it
			if (cunit->_unitData->_isUnderConstruction)
				cunit->GetConstructor()->AbortConstructingUnit();
			//if there are placed any ground unit-destroy it
			else if (!UnitCanStillBePlacedToCell(cell.x, cell.y, cunit->GetConfig(), cunit->_owner_w, true))
				_gameController->DestroyUnit(cunit);
			//else-just update frames
			else
				cunit->CheckBodyAndShadow();
		}
	}
	else if (unit->_unitData->GetIsBuilding() && !unit->_unitData->_isUnderConstruction)
	{
		//else - if building has been destroyed - destroy platforms
		//or destroy them
		vector<CCPoint>cells;
		cells.push_back(point);
		if (unit->GetConfig()->_bSize == 2)
		{
			cells.push_back(ccp(point.x+1, point.y));
			cells.push_back(ccp(point.x, point.y+1));
			cells.push_back(ccp(point.x+1, point.y+1));
		}
		for (int i = 0; i < cells.size(); i++)
		{
			CCPoint cell = cells[i];
            bool find = true;
            while (find) {
                find = false;
                USimpleContainer<GameUnit*> *units_ = _fullAgregator->UnitsInCell(cell.x, cell.y);
                for (int j = 0; j < units_->GetCount(); j++)
                {
                    GameUnit *cunit = units_->objectAtIndex(j);
                    if (cunit->GetConfig()->_isPlatform)
                    {
                        cunit->Destroy(true);
                        find = true;
                        break;
                    }
                }
            }
			
		}
	}
}

void GameMatch::GameUnitDidEnterCell(GameUnit *unit, const CCPoint &point)
{
    _fullAgregator->AddUnitToCell(unit, point.x, point.y);
    _fireAgregator->UnitDidEnterCell(unit, point);
    bool needMessage = false;
	//fill player's agregator
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player = _players[i];
        bool canSee = player->CanSeeUnit(unit);
        //_detected
        if (canSee)
        {
            player->_agregator->AddUnitToCell(unit, point.x, point.y);
            UpdateConnectorsForUnit(unit);
        }
    }
    
    if (_currentPlayer_w->CanSeeUnit(unit))
    {
		needMessage = !unit->_onDraw && unit->_owner_w != _currentPlayer_w && !unit->_unitData->_isUnderConstruction  && !unit->_unitData->GetUniteractable();
        unit->Show();
    }
    else
        unit->Hide();
    
    if (needMessage)
		_gameController->ShowUnitSpottedMessage(unit);
    
	//fill player's agregator
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* player = _players[i];
        if (player == unit->_owner_w) 
            unit->DetectedByPlayer(player->GetPlayerId());
        else if (PlayerIsEnemyToPlayer(player, unit->_owner_w))
        {
            if (player->CanSeeUnit(unit))
            {
                unit->DetectedByPlayer(player->GetPlayerId());
            }
            else
            {
                //TODO: if submarine did fire or stealth unit did detected becouse it was not escape from its cell - stay on sight during this turn if can see with simple scan
                unit->UndetectedByPlayer(player->GetPlayerId());
            }
            
        }
        else //friendly
            unit->DetectedByPlayer(player->GetPlayerId());
    }
    
	if (unit->_unitData->GetIsBuilding())
	{
		vector<CCPoint>cells;
		cells.push_back(point);
		if (unit->GetConfig()->_bSize == 2)
		{
			cells.push_back(ccp(point.x+1, point.y));
			cells.push_back(ccp(point.x, point.y+1));
			cells.push_back(ccp(point.x+1, point.y+1));
		}
		vector<GameUnit*> units;
		for (int i = 0; i < cells.size(); i++)
		{
			USimpleContainer<GameUnit*> *units_ = _fullAgregator->UnitsInCell(cells[i].x, cells[i].y);
			for (int j = 0; j < units_->GetCount(); j++)
				units.push_back(units_->objectAtIndex(j));
		}

       
		if (!unit->_unitData->_isUnderConstruction)
		{
			if (unit->GetConfig()->_isPlatform)
			{
				//if there are any another building- make this platform uninteractable
				for (int i = 0; i < units.size(); i++)
				{
					GameUnit *cunit = units[i];
					if (cunit != unit && cunit->_unitData->GetIsBuilding() && !cunit->_unitData->_isUnderConstruction)
						unit->_unitData->SetIsUniteractable(true);
				}
			}
			else
			{

				//if it is constructed building - make platforms uninteractable
				for (int i = 0; i < units.size(); i++)
				{
					GameUnit *cunit = units[i];
					if (cunit->GetConfig()->_isPlatform && !unit->GetConfig()->_isBombMine)
						cunit->_unitData->SetIsUniteractable(true);
                    if (cunit->GetConfig()->_isRoad && cunit != unit && !unit->GetConfig()->_isBombMine)
                        cunit->Destroy(true);
                    if (cunit->GetConfig()->_isConnector && cunit != unit && unit->_unitData->GetIsConnectored())
                        cunit->Destroy(true);
				}
			}
		}
	}

    CheckAutofire(unit, point);
	
}

void GameMatch::CheckAutofire(GameUnit *unit, const CCPoint &point)
{
    if (_holdAutofire) 
        return;
	
    vector<CCPoint> points;
    points.push_back(point);
    
    if (unit->GetConfig()->_bSize > 1) {
        points.push_back(ccp(point.x + 1, point.y));
        points.push_back(ccp(point.x, point.y + 1));
        points.push_back(ccp(point.x + 1, point.y + 1));
    }
    for (int i = 0; i < points.size(); i++)
    {
        CCPoint &currentPoint = points[i];
        vector<GameUnit*> potentialAttackers = _fireAgregator->UnitsForAttackingUnitInCell(currentPoint.x, currentPoint.y, unit);
        
        vector<GameUnit*> attackers;
        for (int i = 0; i < potentialAttackers.size(); i++)
        {
            GameUnit* cUnit = potentialAttackers[i];
            
            //if cunit is hidden
            if ((!cUnit->_unitData->_detected[unit->_owner_w->_playerData->_playerInfo._playerId] && (cUnit->GetConfig()->_isStealthable || cUnit->GetConfig()->_isStealth || cUnit->GetConfig()->_isUnderwater)) && !cUnit->GetConfig()->_isBombMine)
                continue;
            
            //        if (!cUnit->_unitData->_isOnSentry)
            //            continue;
            //        if (!cUnit->_unitData->_isDisabledByInfiltrator)
            //            continue;
            
            //mines doesnt explode becouse of enemy roads bridges connectors waterplaatforms and etc
            if (cUnit->GetConfig()->_isBombMine && (unit->GetConfig()->_isConnector || unit->GetConfig()->_isPlatform || unit->GetConfig()->_isRoad || unit->GetConfig()->_isBridge)) 
                continue;
            
            //mines doesnt explode becouse of minelayers who removing mines
            if (cUnit->GetConfig()->_isBombMine && (unit->GetConfig()->_isBombMinelayer && unit->_unitData->GetIsRemovingMines()))
                continue;
            
            if (cUnit->_unitData->_disabledByInfiltrator)
                continue;
            
            if (cUnit->GetConfig()->_isInfiltrator)
                continue;
            
            if (cUnit->_unitData->GetParameterValue(UNIT_PARAMETER_TYPE_AMMO) == 0 && !cUnit->GetConfig()->_isBombMine)
                continue;
            
            if (cUnit->_unitData->GetShotBalance() == 0 && !cUnit->GetConfig()->_isBombMine)
                continue;
            
            if (!unit->IsDetectedByPlayer(cUnit->_owner_w->GetPlayerId()) && !cUnit->GetConfig()->_isBombMine)
                continue;
            
            //currently this unit is moving
            if (cUnit->GetPath().size() > 0) 
                continue;
            
            
            attackers.push_back(cUnit);
        }
        if (attackers.size() > 0) 
            _gameController->StartMultipleAttackSequence(attackers, unit, currentPoint, false);
	}
}

void GameMatch::CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag, GameMatchPlayer* player)
{
    if (player->GetIsCurrentPlayer())
    {
        if (type == FOG_TYPE_RESOURCES && visibleFlag)
        {
            engine->AddResourceCell(x, y, _resources->GetResourceTypeAt(x, y), _resources->GetResourceValueAt(x, y));
        }
        else if (type == FOG_TYPE_SCAN)
        {
            engine->AddFogCell(x, y, !visibleFlag);
        }
    }
    
    USimpleContainer<GameUnit*> *units = _fullAgregator->UnitsInCell(x, y);
    
    for (int i = 0; i < _players.size(); i++)
    {
        GameMatchPlayer* processedPlayer = _players[i];
        if (visibleFlag)
        {
            bool needMessage = false;
            for (int i = 0; i < units->GetCount(); i++)
            {
                GameUnit *unit = units->objectAtIndex(i);
				if (processedPlayer->CanSeeUnit(unit) && processedPlayer != unit->_owner_w)
                {
                    needMessage = !unit->_onDraw &&  unit->_owner_w != _currentPlayer_w && !unit->_unitData->_isUnderConstruction && processedPlayer == _currentPlayer_w;
                    if (needMessage)
						_gameController->ShowUnitSpottedMessage(unit);
                    
                    
                    if (type != FOG_TYPE_SCAN)
                        unit->DetectedByPlayer(processedPlayer->GetPlayerId());
                    
                    if (processedPlayer == _currentPlayer_w)
                        unit->Show();
                    
                    CCPoint p = unit->GetUnitCell();
                    processedPlayer->_agregator->AddUnitToCell(unit, p.x, p.y);
                }
            }
        }
        else
        {
            for (int i = 0; i < units->GetCount(); i++)
            {
                GameUnit *unit = units->objectAtIndex(i);
                if (!processedPlayer->CanSeeUnit(unit) && (!unit->GetConfig()->_isUnderwater || processedPlayer->_playerData->FogValueInCell(FOG_TYPE_SCAN, ccp(x, y)) == 0))
                {
                    unit->UndetectedByPlayer(processedPlayer->GetPlayerId());
                    processedPlayer->_agregator->RemoveUnitFromCell(unit, x, y);
                    
                    if (processedPlayer->GetIsCurrentPlayer())
                        unit->Hide();
                    
                    //TODO
                    //check if noone ca see this unit - activate stealth
                    for (int i = 0; i < _players.size(); i++)
                    {
                        GameMatchPlayer* processedPlayer = _players[i];
                    }
                }
            }
        }
    }
}

bool GameMatch::GetIsCellValid(CCPoint cell) const
{
    return cell.x>=0 && cell.y>=0 && cell.x <_map->_w && cell.y < _map->_h;
}

bool GameMatch::GetCanConstructBuildingInCell(const CCPoint &cell, MAXObjectConfig *buildingType, GameUnit *constructor)
{
    //TODO:
    vector<CCPoint>cells;
    cells.push_back(cell);
    if (buildingType->_bSize > 1) {
        cells.push_back(ccp(cell.x+1, cell.y));
        cells.push_back(ccp(cell.x, cell.y+1));
        cells.push_back(ccp(cell.x+1, cell.y+1));
    }
    for (int i = 0; i < cells.size(); i++) {
        CCPoint cell1 = cells[i];
        if (!UnitCanBePlacedToCell(cell1.x, cell1.y, buildingType, constructor->_owner_w) || constructor->_owner_w->_agregator->IsBombMineInPosition(cell1.x, cell1.y))
            return false;
    }
    
    return true;
}

bool GameMatch::IsHiddenUnitInPos(const int x, const int y, const bool checkOnly, GameMatchPlayer *reasonPlayer, vector<CCPoint> lockedCells)
{
	bool result = false;
    
    USimpleContainer<GameUnit*> *units = _fullAgregator->UnitsInCell(x, y);
    for (int i = 0; i < units->GetCount(); i++)
    {
        GameUnit* unit = units->objectAtIndex(i);
        if (unit->GetIsStealthable())
        {
            if (checkOnly)
            {
                result = true;
                break;
            }
            else
            {
                // try to quick move if possible to prevent detecting
				bool escaped = escaped = _gameController->EscapeStealthUnitFromPos(unit, x, y, reasonPlayer, lockedCells);
                if (!escaped)
                {
                    result = true;
					_gameController->ShowUnitSpottedMessage(unit);
                    unit->DetectedByPlayer(_currentPlayer_w->GetPlayerId());
                    unit->Show();
                    reasonPlayer->_agregator->AddUnitToCell(unit, x, y);
                    break;
                }
            }
        }
	}
	return result;
}

bool GameMatch::UnitCanBePlacedToCell(const int x, const int y, MAXObjectConfig *buildingType, GameMatchPlayer* player)
{
	return UnitCanStillBePlacedToCell(x, y, buildingType, player, false);
}

bool GameMatch::UnitCanStillBePlacedToCell(const int x, const int y, MAXObjectConfig *buildingType, GameMatchPlayer* player, bool alreadyPlaced)
{
    UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)buildingType->_bMoveType;
    EXTENDED_GROUND_TYPE groundType = player->_agregator->GroundTypeAtXY(x, y);
    GROUND_TYPE mapGroundType = _map->GroundTypeAtXY(x, y);
    bool waterPlatformFlag = true;
    if (buildingType->_isBuilding                           &&
        buildingType->_bMoveType == UNIT_MOVETYPE_GROUND    &&
        mapGroundType != GROUND_TYPE_GROUND)
        waterPlatformFlag = _fullAgregator->IsWaterPlatformInPosition(x, y);
    
    
    switch (unitMoveType)
    {
        case UNIT_MOVETYPE_GROUND:
        {
            if ((groundType == EXTENDED_GROUND_TYPE_ROAD) ||
                (groundType == EXTENDED_GROUND_TYPE_BRIDGE) ||
                (groundType == EXTENDED_GROUND_TYPE_GROUND))
				return waterPlatformFlag && (!player->_agregator->IsGroundUnitInPosition(x, y) || alreadyPlaced);
            break;
        }
        case UNIT_MOVETYPE_GROUNDCOAST:
        {
            if ((groundType == EXTENDED_GROUND_TYPE_ROAD) ||
                (groundType == EXTENDED_GROUND_TYPE_BRIDGE) ||
                (groundType == EXTENDED_GROUND_TYPE_GROUND) ||
                (groundType == EXTENDED_GROUND_TYPE_COAST))
                return !player->_agregator->IsGroundUnitInPosition(x, y) || alreadyPlaced;
            break;
        }
        case UNIT_MOVETYPE_SURVEYOR:
        {
            if ((groundType == EXTENDED_GROUND_TYPE_ROAD) ||
                (groundType == EXTENDED_GROUND_TYPE_BRIDGE) ||
                (groundType == EXTENDED_GROUND_TYPE_GROUND) ||
                (groundType == EXTENDED_GROUND_TYPE_COAST) ||
                (groundType == EXTENDED_GROUND_TYPE_WATER))
                return !player->_agregator->IsGroundUnitInPosition(x, y) || alreadyPlaced;
            break;
        }
        case UNIT_MOVETYPE_AMHIB:
        {
            if ((groundType == EXTENDED_GROUND_TYPE_ROAD) ||
                (groundType == EXTENDED_GROUND_TYPE_BRIDGE) ||
                (groundType == EXTENDED_GROUND_TYPE_GROUND) ||
                (groundType == EXTENDED_GROUND_TYPE_COAST)||
                (groundType == EXTENDED_GROUND_TYPE_WATER))
                return !player->_agregator->IsGroundUnitInPosition(x, y) || alreadyPlaced;
            break;
        }
        case UNIT_MOVETYPE_SEACOAST:
        {
            if ((groundType == EXTENDED_GROUND_TYPE_WATER) ||
                (groundType == EXTENDED_GROUND_TYPE_COAST) ||
                (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                return !player->_agregator->IsGroundUnitInPosition(x, y) || alreadyPlaced;
            break;
        }
        case UNIT_MOVETYPE_SEA:
        {
            if ((groundType == EXTENDED_GROUND_TYPE_WATER) ||
                (groundType == EXTENDED_GROUND_TYPE_BRIDGE))
                return !player->_agregator->IsGroundUnitInPosition(x, y) || alreadyPlaced;
            break;
        }
        case UNIT_MOVETYPE_AIR:
        {
            return !player->_agregator->IsAirUnitInPosition(x, y) || alreadyPlaced;
            break;
        }
    }
    return false;
}

