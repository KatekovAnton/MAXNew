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
#include "UserInterface.h"

#include "MAXConfigManager.h"
#include "MAXContetnLoader.h"
#include "MAXUnitObject.h"

#include "GameMap.h"
#include "GameUnit.h"
#include "GameMatch.h"
#include "GameInteface.h"

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
    
    //Aagunm
    //Tank
    //Inter
    //Bomber
    //Awac
    //Scout
    //Asgun
    _currentUnit = NULL;
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 56, "Inter", 0);
        engine->AddUnit(unit1->GetUnitObject());
    }
    {
        GameUnit *unit1 = _match->_players[1]->CreateUnit(50, 56, "Inter", 0);
        engine->AddUnit(unit1->GetUnitObject());
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(54, 55, "Inter", 0);
        engine->AddUnit(unit1->GetUnitObject());
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(54, 58, "Inter", 0);
        engine->AddUnit(unit1->GetUnitObject());
    }
    {
        GameUnit *unit1 = _match->_players[1]->CreateUnit(50, 53, "Inter", 0);
        engine->AddUnit(unit1->GetUnitObject());
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
    
    CCRect r = CCRect(0, 280, 130, 30);
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

void MAXGame::ProceedTap(float tapx, float tapy)
{
    bool _unitMoved = false;
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
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
            if (groundType == GROUND_TYPE_GROUND || _currentUnit->_config->_bLevel == UNIT_LEVEL_AIR)
            {
                _currentUnit->SetUnitLocation(p, true);
                _unitMoved = true;
            }
        }
    }
    if (!_unitMoved)
    {
        if (_currentUnit) 
            engine->SelectUnit(NULL);
        
        

        _currentUnit = _match->_currentPlayer_w->GetUnitInPosition(p);
        if (_currentUnit)
            engine->SelectUnit(_currentUnit->GetUnitObject());
        
    }
    
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
    if (_currentUnit) {
        CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
        _currentUnit->Fire(p);
    }
   ///engine->SelectUnit(_currentUnit->GetUnitObject());
}







