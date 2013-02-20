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
    _match = new GameMatch("UnitListOriginal.txt", "Green_6.wrl");
    engine->SetMap(_match->_map->_contentMap);
    
    //Aagunm
    //Tank
    //Inter
    //Bomber
    //Awac
    //Scout
    //Asgun
    MAXUnitConfig* unit = MAXConfigManager::SharedMAXConfigManager()->GetConfig("Inter");
    _testUnit = shared_ptr<GameUnit>(new GameUnit(MAXSCL->CreateUnit(unit), unit));
    _testUnit->SetUnitLocation(CCPoint(56, 56), false);
    engine->AddUnit(_testUnit->GetUnitObject());
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
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    CCPoint location = _testUnit->GetUnitCell();
    if (p.x < 0 || p.x>= _match->_map->GetMapWidth() || p.y < 0 || p.y >= _match->_map->GetMapHeight())
        return;
    if ((!(p.x == location.x && p.y == location.y)) &&                          //not same
        (fabsf(p.x - location.x) <= 1 || fabsf(p.y - location.y) <= 1) &&       //only near
        (fabsf(p.x - location.x) < 2 && fabsf(p.y - location.y) < 2))           //only
    {
        char groundType = _match->_map->GroundTypeAtPoint(p);
        if (groundType == GROUND_TYPE_GROUND || _testUnit->_config->_bLevel == UNIT_LEVEL_AIR)
            _testUnit->SetUnitLocation(p, true);
    }
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    _testUnit->Fire(p);
    
    engine->SelectUnit(_testUnit->GetUnitObject());
}







