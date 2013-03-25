//
//  MAXGame.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXGame.h"
#include "MAXEngine.h"
#include "SceneSystem.h"
#include "Display.h"
#include "cocos2d.h"

#include "MAXEffectObject.h"
#include "MAXConfigManager.h"
#include "MAXContentLoader.h"
#include "MAXUnitObject.h"

#include "GameMap.h"
#include "GameUnit.h"
#include "GameMatch.h"
#include "GameEffect.h"
#include "GameInteface.h"
#include "GameUnitParameters.h"

MAXGame globalGame;
MAXGame * game = &globalGame;

MAXGame::MAXGame()
{
    _currentState = MAXGAMESTATE_GAME;
    _effects = new USimpleContainer<GameEffect*>();
    if (Display::currentDisplay()->GetDisplayScale() == 1.0) {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("simple");
    }
    if (Display::currentDisplay()->GetDisplayScale() == 2.0) {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("retina");
    }
}

MAXGame::~MAXGame()
{
    engine->_delegate = NULL;
    delete _gameInterface;
    for (int i = 0; i < _effects->GetCount(); i++) {
        GameEffect *effect = _effects->objectAtIndex(i);
        delete effect;
    }
    delete _effects;
}

void MAXGame::Init()
{
    Display::currentDisplay()->SetPinchDelegate(this);
    engine->_delegate = this;
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
    
    _currentUnit = NULL;
    
//    for (int x = 0; x < 100; x++)
//    {
//        for (int y = 0; y < 100; y++)
//        {
//            GameUnit *unit1 = _match->_players[0]->CreateUnit(x, y, "Inter", 0);
//            unit1->SetRandomDirection();
//            unit1->LocateOnMap();
//        }
//    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 56, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
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
        GameUnit *unit1 = _match->_players[0]->CreateUnit(78, 45, "pcan", 0);
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
        GameUnit *unit1 = _match->_players[0]->CreateUnit(48, 58, "Engineer", 0);
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
        GameUnit *unit1 = _match->_players[0]->CreateUnit(53, 54, "Rocket", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }

    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 58, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->LocateOnMap();
    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(68, 45, "Airplant", 0);
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(68, 43, "Hvplant", 0);
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(65, 43, "Hang", 0);
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(63, 43, "Powerpl", 0);
        unit1->LocateOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(80, 41, "Shipyard", 0);
        unit1->LocateOnMap();
    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(50, 64, "Scanner", 0);
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

void MAXGame::FlushEffectsWithNew(GameEffect *effect)
{
    bool onemorestep = true;
    while (onemorestep)
    {
        onemorestep = false;
        int count = _effects->GetCount();
        for (int i = 0; i < count; i++)
        {
            GameEffect* effect = _effects->objectAtIndex(i);
            if (effect->GetFinished())
            {
                _effects->remove(i);
                onemorestep = true;
                delete effect;
                break;
            }
        }
    }
}

#pragma mark - MAXEngineDelegate

void MAXGame::onFrame()
{
    for(int i = 0; i < _match->_players.size(); i++)
        _match->_players[i]->SetPalette(engine->FullTime());
}

#pragma mark - DisplayPinchDelegate

bool MAXGame::CanStartPinch(float x, float y)
{
    switch (_currentState)
    {
        case MAXGAMESTATE_GAME:
        {
            float _y = Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale() - y;
            return _gameInterface->ShouldReceiveTouch(x, _y);
        }  break;
            
        default:
            break;
    }
    return false;
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
    switch (unitType)
    {
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
    if (_currentUnit && !_currentUnit->_config->GetConfig()->_isBuilding)
    {
        
        CCPoint location = _currentUnit->GetUnitCell();
        if (p.x < 0 || p.x>= _match->_map->GetMapWidth() || p.y < 0 || p.y >= _match->_map->GetMapHeight())
        {}
        else if ((!(p.x == location.x && p.y == location.y)) &&                          //not same
                 (fabsf(p.x - location.x) <= 1 || fabsf(p.y - location.y) <= 1) &&       //only near
                 (fabsf(p.x - location.x) < 2 && fabsf(p.y - location.y) < 2))           //only
        {
            char groundType = _match->_map->GroundTypeAtPoint(p);
            int unitMoveType = _currentUnit->_config->GetConfig()->_bMoveType;
            
            if (CanMove(unitMoveType, groundType, newCurrentUnit != NULL))
            {
                _currentUnit->SetUnitLocationAnimated(p);
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
            _gameInterface->OnCurrentUnitChanged(newCurrentUnit);
        }
        
        if (!newCurrentUnit)
        {
            _gameInterface->OnCurrentUnitChanged(NULL);
            engine->SelectUnit(NULL);
            _currentUnit = NULL;
        }
    }
    
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
    if (_currentUnit)
    {
        if (_currentUnit->CanStartBuildProcess())
        {
            _currentUnit->StartBuildProcess();
        }
        else
        {
            CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
            _currentUnit->Fire(p);
        }
        
    }
}







