//
//  MAXGame.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXGame.h"
#include "SceneSystem.h"
#include "MAXEngine.h"
#include "Display.h"

#include "MAXConfigManager.h"
#include "MAXContetnLoader.h"
#include "MAXUnitObject.h"

#include "GameMap.h"
#include "GameUnit.h"
#include "GameMatch.h"
#include "GameInteface.h"
#include "GameUnitParameters.h"

MAXGame globalGame;
MAXGame * game = &globalGame;

MAXGame::MAXGame()
{
    _curretnState = MAXGAMESTATE_GAME;
}

MAXGame::~MAXGame()
{
    delete _gameInterface;
}

void MAXGame::Init()
{
    Display::currentDisplay()->SetPinchDelegate(this);
    
    StartMatch();
}

void MAXGame::StartMatch()
{
    vector<GameMatchPlayerInfo> infos;
    GameMatchPlayerInfo player1 = {0, 3, "Test player3", {180,0,0,255}};
    GameMatchPlayerInfo player2 = {0, 7, "Test player7", {0,180,0,255}};
    infos.push_back(player1);
    infos.push_back(player2);
    _match = new GameMatch("UnitListOriginal.txt", "Green_6.wrl", infos);
    engine->SetMap(_match->_map->_contentMap);
    
//    Aagunm=grp_mobile_aa_gun.cfg#mobile_aa_gun.cfg
//    Asgun=grp_assault_gun.cfg#assault_gun.cfg
//    Awac=grp_awac.cfg#awac.cfg
//    Bomber=grp_bomber.cfg#bomber.cfg
//    Constructor=grp_constructor.cfg#constructor.cfg
//    Corvette=grp_corvette.cfg#corvette.cfg
//    Dozer=grp_bulldozer.cfg#bulldozer.cfg
//    Engineer=grp_engineer.cfg#engineer.cfg
//    Escort=grp_escort.cfg#escort.cfg
//    Gunboat=grp_gun_boat.cfg#gun_boat.cfg
//    Inter=grp_interceptor.cfg#interceptor.cfg
//    Tank=grp_tank.cfg#tank.cfg
//    Scout=grp_scout.cfg#scout.cfg
//    sub=grp_submarine.cfg#submarine.cfg
//    pcan=grp_personnel_carrier.cfg#personnel_carrier.cfg
    _currentUnit = NULL;
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(60, 53, "Constructor", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(45, 47, "Corvette", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(63, 60, "Dozer", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(62, 58, "Engineer", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(47, 49, "Escort", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(44, 49, "Gunboat", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(44, 44, "sub", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(74, 45, "pcan", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 56, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 52, "Bomber", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 57, "Aagunm", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 58, "Awac", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(58, 58, "Scout", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(59, 53, "Asgun", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 54, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(55, 52, "Tank", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 58, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[1]->CreateUnit(50, 56, "Inter", 0);
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[1]->CreateUnit(50, 53, "Inter", 0);
        unit1->LocateOnMap();
    }
    
    _gameInterface = new GameInterface();
    _gameInterface->InitBaseInterface();
    CCDirector::sharedDirector()->pushScene(_gameInterface);
}

void MAXGame::SetMap(string mapName)
{
    
    
}

#pragma mark - DisplayPinchDelegate

bool MAXGame::CanStartPinch(float x, float y)
{
    float _y = Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale() - y;
    
    CCRect r = CCRect(0, 220, 130, 100);
    return !r.containsPoint(CCPoint(x,_y));
   // return _curretnState == MAXGAMESTATE_GAME;
}

void MAXGame::ProceedPinch(float scale)
{
    engine->ScaleCamera(scale);
}

void MAXGame::ProceedPan(float speedx, float speedy)
{
    engine->MoveCamera(speedx, speedy);
}

bool CanMove(int unitType, int groundType, bool unitInCell)
{
    switch (unitType) {
        case UNIT_MOVETYPE_GROUND:
            return groundType == GROUND_TYPE_GROUND && !unitInCell;
            break;
        case UNIT_MOVETYPE_GROUNDCOST:
            return (groundType == GROUND_TYPE_GROUND || groundType == GROUND_TYPE_COAST) && !unitInCell;
            break;
        case UNIT_MOVETYPE_AMHIB:
            return (groundType == GROUND_TYPE_WATER || groundType == GROUND_TYPE_COAST || groundType == GROUND_TYPE_GROUND)  && !unitInCell;
            break;
        case UNIT_MOVETYPE_SEACOST:
            return (groundType == GROUND_TYPE_WATER || groundType == GROUND_TYPE_COAST) && !unitInCell;
            break;
        case UNIT_MOVETYPE_SEA:
            return groundType == GROUND_TYPE_WATER && !unitInCell;
            break;
        case UNIT_MOVETYPE_AIR:
            return true;
            break;
        default:
            break;
    }
    return true;
}

void MAXGame::ProceedTap(float tapx, float tapy)
{
    bool _unitMoved = false;
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    GameUnit* newCurrentUnit = _match->_currentPlayer_w->GetUnitInPosition(p);
    if (_currentUnit)
    {
        
        CCPoint location = _currentUnit->GetUnitCell();
        if (p.x < 0 || p.x>= _match->_map->GetMapWidth() || p.y < 0 || p.y >= _match->_map->GetMapHeight())
        {}
        else if ((!(p.x == location.x && p.y == location.y)) &&                          //not same
                 (fabsf(p.x - location.x) <= 1 || fabsf(p.y - location.y) <= 1) &&       //only near
                 (fabsf(p.x - location.x) < 2 && fabsf(p.y - location.y) < 2))           //only
        {
            char groundType = _match->_map->GroundTypeAtPoint(p);
            int unitMoveType = _currentUnit->_config->GetCongig()->_bMoveType;
            
            if (CanMove(unitMoveType, groundType, newCurrentUnit != NULL))
            {
                _currentUnit->SetUnitLocation(p, true);
                _unitMoved = true;
            }
        }
    }
    if (!_unitMoved)
    {
        
        
        if (newCurrentUnit && _currentUnit != newCurrentUnit)
        {
            _currentUnit = newCurrentUnit;
            engine->SelectUnit(_currentUnit->GetUnitObject());
        }
        
        if (!newCurrentUnit)
        {
            engine->SelectUnit(NULL);
            _currentUnit = NULL;
        }
    }
    
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
    if (_currentUnit)
    {
        CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
        _currentUnit->Fire(p);
    }
}







