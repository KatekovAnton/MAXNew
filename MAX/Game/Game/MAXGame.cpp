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
#include "SoundEngine.h"
#include "Display.h"
#include "cocos2d.h"

#include "MAXEffectObject.h"
#include "MAXConfigManager.h"
#include "MAXContentLoader.h"
#include "MAXUnitObject.h"
#include "MAXAnimationPrefix.h"

#include "MatchMapAgregator.h"

#include "GameMap.h"
#include "GameFog.h"
#include "GameUnit.h"
#include "GameMatch.h"
#include "GameEffect.h"
#include "GameInteface.h"
#include "GameUnitParameters.h"
#include "GameUnitCurrentState.h"
#include "GamePathVisualizer.h"

#include "Pathfinder.h"
#include "PFWaveCell.h"

MAXGame globalGame;
MAXGame * game = &globalGame;

MAXAnimationWait* prepareUnitToMoveToPoint(GameUnit* unit, CCPoint point, float delay)
{
    CCPoint location = unit->GetUnitCell();
    UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
    Pathfinder* pf = game->_match->_pathfinder;
    pf->MakePathMap(location.x, location.y, unitMoveType, unit->_unitCurrentParameters->GetMoveBalance());
    std::vector<PFWaveCell*> path = pf->FindPathOnMap(point.x, point.y); // call after MakePathMap
    
    if (path.size() > 1)
    {
        unit->SetPath(path);
        game->ShowUnitPath(unit);
    }
    MAXAnimationWait* animWait = new MAXAnimationWait(delay);
    animWait->_delegate = game;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(animWait);
    return animWait;
}

MAXAnimationWait* moveUnit(GameUnit* unit, float delay)
{
    if (game->CheckIfNextCellOk(unit))
    {
        unit->ConfirmCurrentPath();
        game->HideUnitPath();
        MAXAnimationWait* animWait = new MAXAnimationWait(delay);
        animWait->_delegate = game;
        MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(animWait);
        return animWait;
    }
    return NULL;
}

MAXGame::MAXGame()
:_testUnitCorvette(NULL), iteration(0), _pathVisualizer(NULL), _freezeCounter(0)
{
    _currentState = MAXGAMESTATE_GAME;
    _effects = new USimpleContainer<GameEffect*>();
    _pathVisualizer = new GamePathVisualizer();
}

MAXGame::~MAXGame()
{
    engine->_delegate = NULL;
    delete _gameInterface;
    for (int i = 0; i < _effects->GetCount(); i++) {
        GameEffect *effect = _effects->objectAtIndex(i);
        delete effect;
    }
    delete _pathVisualizer;
    delete _effects;
}

void MAXGame::Init()
{
    Display::currentDisplay()->SetPinchDelegate(this);
    engine->_delegate = this;
    StartMatch();
    //StartTest();
}

void MAXGame::StartTest()
{
    _waitTestAnimCorvette = NULL;
    _waitTestAnimSubmarine = NULL;
    _waitTestAnimCorvetteMovement = NULL;
    _waitTestAnimSubmarineMovement = NULL;
    if (!_testUnitCorvette)
        return;
    
    _testUnitCorvette->SetDirection(1);
    
    _waitTestAnimCorvette = prepareUnitToMoveToPoint(_testUnitCorvette, ccp(26, 44), 2.5);
    _freezeCounter ++;
    

}

void MAXGame::ShowPathMap()
{
    Pathfinder* pf = _match->_pathfinder;
    for (int x = 0; x < _match->_map->GetMapWidth(); x++)
    {
        for (int y = 0; y < _match->_map->GetMapHeight(); y++)
        {
            int cost = pf->GetCostAt(x, y);
            if (cost == 0)
            {
                // current unit pos - don't highlight
            }
            else if (cost > 0)
            {
                // valid map cost - highlight with green aquare
				engine->AddPathZoneCell(x, y);
            }
            else
            {
                // unpassable - don't highlight
            }
        }
    }
}

void MAXGame::HidePathMap()
{
	engine->ClearPathZone();
}


void MAXGame::StartMatch()
{
    vector<GameMatchPlayerInfo> infos;
    GameMatchPlayerInfo player1 = {3, 3, "Test player3", {255,0,0,255}};
    GameMatchPlayerInfo player2 = {0, 7, "Test player7", {0,255,0,255}};
    infos.push_back(player1);
    infos.push_back(player2);
    _match = new GameMatch("UnitListOriginal.txt", "clansOriginal.cfg", "Green_6.wrl", infos);
    
    _currentUnit = NULL;
    
    
    
    
    _gameInterface = new GameInterface();
    _gameInterface->InitBaseInterface();
    CCDirector::sharedDirector()->pushScene(_gameInterface);

    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 56, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(60, 53, "Constructor", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(45, 47, "Corvette", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    } 
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(47, 49, "Escort", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(44, 49, "Gunboat", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(44, 44, "sub", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(45, 45, "Seacargo", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(46, 45, "Seafuel", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(45, 46, "Rokcr", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(44, 42, "seaminelay", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(45, 43, "Seatrans", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
        GameUnit *unit1 = _match->_players[0]->CreateUnit(78, 45, "pcan", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 52, "Bomber", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(58, 52, "Airtrans", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 53, "Crawler", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 51, "infantry", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(55, 51, "Infil", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 57, "Aagunm", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 58, "Awac", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(10, 10, "Awac", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(101, 101, "Awac", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(58, 58, "Scout", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(48, 58, "Engineer", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(59, 53, "Asgun", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 54, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(55, 52, "Tank", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(53, 54, "Rocket", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(56, 58, "Inter", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(55, 58, "Surveyor", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(65, 58, "Dozer", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(65, 59, "Fueltruck", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(66, 59, "GTruck", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(67, 59, "Truck", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(66, 60, "Repair", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
		GameUnit *unit1 = _match->_players[0]->CreateUnit(73, 52, "Miner", 0);
		unit1->SetRandomDirection();
		unit1->PlaceUnitOnMap();
	}
	{
        GameUnit *unit1 = _match->_players[0]->CreateUnit(66, 58, "alnasgun", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(66, 57, "alnplane", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(64, 55, "alntank", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(44, 33, "juger", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(50, 64, "Scanner", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        _match->_players[1]->CreateUnit(25, 58, "Surveyor", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 43, "Goldstore", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 44, "landpad", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 45, "Matstore", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 46, "Powergen", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 47, "Fuelstore", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(68, 43, "Hvplant", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(65, 43, "Hang", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(63, 43, "Powerpl", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(68, 45, "Airplant", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(70, 45, "gref", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(68, 47, "Conn", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(68, 48, "Conn", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(80, 38, "Dock", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(65, 42, "Radar", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(66, 42, "turret", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(64, 42, "Zenit", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(63, 42, "Arturret", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(62, 42, "misturret", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(68, 49, "Lightplant", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(66, 49, "barrak", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(64, 49, "Ecosphere", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(62, 49, "Habitat", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(60, 49, "Store", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(65, 46, "Mining", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(63, 46, "research", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(61, 46, "pehplant", 0)->PlaceUnitOnMap();
        for (int i = 68; i < 79; i++)
            _match->_players[0]->CreateUnit(i, 42, "Conn", 0)->PlaceUnitOnMap();
        for (int i = 60; i < 70; i++)
            _match->_players[0]->CreateUnit(i, 51, "Conblock", 0)->PlaceUnitOnMap();
		for (int i = 59; i < 71; i++)
			_match->_players[0]->CreateUnit(i, 52, "landmine", 0)->PlaceUnitOnMap();
		for (int i = 30; i < 46; i+=2)
			_match->_players[0]->CreateUnit(i, 36 + ((i / 2) % 2), "seamine", 0)->PlaceUnitOnMap();
		for (int i = 55; i <= 62; i++)
            _match->_players[0]->CreateUnit(i, 43, "Road", 0)->PlaceUnitOnMap();
        for (int i = 28; i <= 34; i++)
            _match->_players[0]->CreateUnit(i, 32, "Plat", 0)->PlaceUnitOnMap();
        for (int i = 41; i <= 47; i++)
            _match->_players[0]->CreateUnit(i, 32, "Plat", 0)->PlaceUnitOnMap();
        for (int i = 35; i <= 40; i++)
            _match->_players[0]->CreateUnit(i, 32, "Bridge", 0)->PlaceUnitOnMap();
        for (int i = 44; i < 49; i++)
            _match->_players[0]->CreateUnit(67, i, "Conn", 0)->PlaceUnitOnMap();

        {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(70, 43, "Powerpl", 0);
            unit1->PlaceUnitOnMap();
            unit1->StartBuildProcess();
        }
        {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(80, 41, "Shipyard", 0);
            unit1->PlaceUnitOnMap();
            vector<CCPoint> points = unit1->GetNerbyCells();
            for (int i = 0; i < points.size(); i++) 
                _match->_players[0]->CreateUnit(points[i].x, points[i].y, "Conn", 0)->PlaceUnitOnMap();
        }
    }
    
    {
        _match->_players[0]->CreateUnit(30, 44, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(31, 43, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(31, 44, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(31, 45, "sub", 0)->PlaceUnitOnMap();
        
        
        _match->_players[1]->CreateUnit(30, 40, "Gunboat", 0)->PlaceUnitOnMap();
        
        _match->_players[1]->CreateUnit(29, 43, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(30, 45, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(29, 45, "Gunboat", 0)->PlaceUnitOnMap();
        
        _testUnitSubmarine = _match->_players[0]->CreateUnit(32, 40, "sub", 0);
        _testUnitSubmarine->PlaceUnitOnMap();
        
        _testUnitCorvette = _match->_players[1]->CreateUnit(24, 44, "Corvette", 0);
        _testUnitCorvette->PlaceUnitOnMap();
    }
    
    
    {
        
        _match->_players[1]->CreateUnit(40, 50, "Corvette", 0)->PlaceUnitOnMap();
            
        
        _match->_players[1]->CreateUnit(39, 55, "Inter", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(38, 55, "Awac", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(50, 53, "Inter", 0)->PlaceUnitOnMap();
        for (int i = 61; i < 69; i++)
            _match->_players[1]->CreateUnit(i, 65, "landmine", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(52, 48, "Infil", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(42, 42, "sub", 0)->PlaceUnitOnMap();
		for (int i = 32; i < 40; i+=2)
			_match->_players[1]->CreateUnit(i, 31, "seamine", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(43, 41, "pcan", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(46, 38, "pcan", 0)->PlaceUnitOnMap();
    }
    
    engine->SetCameraCenter(ccp(30, 44));
    _match->_players[0]->cameraPosition = ccp(30, 44);
    _match->_players[1]->cameraPosition = ccp(30, 44);

}

bool MAXGame::EndTurn()
{
    bool result = false;
    if (_freezeCounter != 0)
        return false;
    _match->_currentPlayer_w->cameraPosition = engine->ScreenToWorldCell( _gameInterface->GetCenter());
    result = _match->EndTurn();
    if (!result) 
        return result;
    
    _gameInterface->SetLockUnits(false);
    _gameInterface->ClearLockedUnits();
    GameUnit* _oldSelectedUnit = _currentUnit;
    if (_oldSelectedUnit)
        HidePathMap();
        
    
    
    _currentUnit = NULL;
    engine->SelectUnit(_currentUnit?_currentUnit->GetUnitObject():NULL);
    _gameInterface->OnCurrentUnitChanged(_currentUnit, false);
    _needToOpenMenuOnNextTapToSameUnit = true;
    _gameInterface->HideUnitMenu();
    _gameInterface->OnCurrentUnitDataChanged(_currentUnit);
    if (_currentUnit) {
        if (_currentUnit->_owner_w->GetIsCurrentPlayer()) {
            ShowUnitPath(_currentUnit);
            
            CCPoint location = _currentUnit->GetUnitCell();
            UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
            Pathfinder* pf = _match->_pathfinder;
            pf->MakePathMap(location.x, location.y, unitMoveType, _currentUnit->_unitCurrentParameters->GetMoveBalance());
            ShowPathMap();
        }
        else
            HidePathMap();
    }
    
    engine->SetCameraCenter(_match->_currentPlayer_w->cameraPosition);
    return result;
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

bool MAXGame::EscapeStealthUnitFromPos(GameUnit* unit, const int x, const int y)
{
    bool result = false;
    MAXObjectConfig* config = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig();
    if (config->_isBuilding)
    {
        if (config->_isBombMine)
        {
            result = true; // bomb always escaped! Unit - welcomme to bomb cell!
        }
    }
    else
    {
        int startDirection = random() % 8;
        int direction = startDirection;
        Pathfinder* pf = _match->_pathfinder;
        UNIT_MOVETYPE movetype = (UNIT_MOVETYPE)config->_bMoveType;
        for (;;)
        {
            int newX = x + PFWaveCell::DirToDX(direction);
            int newY = y + PFWaveCell::DirToDY(direction);
            int cost = pf->GetMapCostAt(newX, newY, direction, movetype);
            if ((cost >= 0) && (cost <= unit->_unitCurrentParameters->GetMoveBalance()))
            {
                if (!_match->IsHiddenUnitInPos(newX, newY, true))
                {
                    unit->EscapeToLocation(newX, newY, cost);
                    result = true;
                    break;
                }
            }
            
            direction++;
            if (direction >= 8)
            {
                direction -= 8;
            }
            if (direction == startDirection)
            {
                break;
            }
        }
    }
    return result;
}

#pragma mark - Interface
#pragma mark Messages
void MAXGame::ShowUnitSpottedMessage(GameUnit* unit)
{
    printf("Enemy %s spotted at %d, %d!\n", unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_name.c_str(), (int)unit->GetUnitCell().x, (int)unit->GetUnitCell().y);
    _gameInterface->ShowUnitSpottedMessage(unit);
}

#pragma mark - MAXEngineDelegate

void MAXGame::onFrame()
{
    SoundEngine::sharedInstance()->CheckStoppedSound();
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
            return !_gameInterface->ShouldReceiveTouch(x, _y);
        }  break;
            
        default:
            break;
    }
    return false;
}

void MAXGame::ProceedPinch(float scale)
{
    engine->ScaleCamera(scale);
    if (_gameInterface->GetUnitMenuOpened()) {
        _gameInterface->HideUnitMenu();
        _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
    }
}

void MAXGame::ProceedPan(float speedx, float speedy)
{
    engine->MoveCamera(speedx, speedy);
    if (_gameInterface->GetUnitMenuOpened()) {
        _gameInterface->HideUnitMenu();
        _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
    }
}

void MAXGame::ShowUnitPath(GameUnit *unit)
{
    std::vector<PFWaveCell*> path = unit->GetPath();
    int pathStep = unit->GetPathIndex();
    int pathSize = path.size();
    if (pathStep > pathSize - 2)
        pathStep = pathSize - 2;
    int speed = unit->_unitCurrentParameters->GetMoveBalance();
	vector<PathElement> testPath;
    int totalCost = 0;
	for (int pi = pathStep; pi >= 0; pi--)
	{
		PFWaveCell* cell = path[pi];
		PathElement element;
		element.x = cell->x;
		element.y = cell->y;
		element.unitLevel = OBJECT_LEVEL_OVERAIR;
		element.image = cell->direction;
        totalCost += cell->cost;
        bool endTurnMarker = false;
        if (pi > 0)
        {
            if ((totalCost < speed) && (totalCost + path[pi - 1]->cost > speed))
            {
                endTurnMarker = true;
            }
        }
        if ((pi == 0) || (totalCost == speed))
        {
            endTurnMarker = true;
        }
        if (endTurnMarker)
        {
            element.image += 8;
        }
		if (totalCost >= speed)
		{
			totalCost -= speed;
			speed = unit->_unitCurrentParameters->GetMaxParameterValue(UNIT_PARAMETER_TYPE_SPEED);
		}
		testPath.push_back(element);
	}
	_pathVisualizer->VisualizePath(testPath);
}

void MAXGame::HideUnitPath()
{
    _pathVisualizer->Clear();
}

void MAXGame::ProceedTap(float tapx, float tapy)
{
    if (_freezeCounter>0) {
        return;
    }
    bool _unitMoved = false;
    bool _unitMenuShowed = false;
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    
 //   printf("(%d, %d) = res=%d, scan=%d\n", (int)p.x, (int)p.y, _match->_currentPlayer_w->_resourceMapFog->GetValue(p), _match->_currentPlayer_w->_fog->GetValue(p));
    bool _removeFromLock = false;
    
    GameUnit* newCurrentUnit = _match->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit);// _currentPlayer_w->GetUnitInPosition(p);
    if (_currentUnit && !_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        if (p.x < 0 || p.x>= _match->_map->GetMapWidth() || p.y < 0 || p.y >= _match->_map->GetMapHeight())
        {}
        else if (_currentUnit == newCurrentUnit)
        {
            if (_needToOpenMenuOnNextTapToSameUnit)
            {
                _gameInterface->ShowMenuForCurrentUni(this);
                _unitMenuShowed = true;
                _needToOpenMenuOnNextTapToSameUnit = false;
            }
            else
            {
                _gameInterface->HideUnitMenu();
                newCurrentUnit = NULL; // deselect current unit
                _removeFromLock = true;
            }
        }
        else
        {
            if (newCurrentUnit)
            {
                if (_currentUnit->GetPath().size() > 0)
                {
                    CCPoint location1 = _currentUnit->GetUnitCell();
                    CCPoint location2 = newCurrentUnit->GetUnitCell();
                    if ((location1.x == location2.x) && (location1.y == location2.y))
                    {
                        // clear path
                        newCurrentUnit = NULL;
                        _unitMoved = true;
                        std::vector<PFWaveCell*> path;
                        _currentUnit->SetPath(path);
                        HideUnitPath();
                    }
                }
                // force select another unit
            }
            else if (_currentUnit->_owner_w->GetIsCurrentPlayer())
            {
                _gameInterface->HideUnitMenu();
                _needToOpenMenuOnNextTapToSameUnit = true;
                if (!_currentUnit->GetIsFreezed())
                {
                    if (_currentUnit->IsPathTargetedTo(p.x, p.y))
                    {
                        if (CheckIfNextCellOk(_currentUnit))
                        {
                            _currentUnit->ConfirmCurrentPath();
                            HideUnitPath();
                            _unitMoved = true;
                        }
                        else
                        {
                            RecalculateUnitPath(_currentUnit);
                            _unitMoved = true;
                        }
                    }
                    else
                    {
                        CCPoint location = _currentUnit->GetUnitCell();
                        Pathfinder* pf = _match->_pathfinder;
                        std::vector<PFWaveCell*> path = pf->FindPathOnMap(p.x, p.y); // call after MakePathMap
                        //pf->DumpMap();
                        if (path.size() > 1)
                        {
                            _currentUnit->SetPath(path);
                            ShowUnitPath(_currentUnit);
                        }
                        else
                        {
                            //engine->SelectUnit(_currentUnit->GetUnitObject());
                            // play error sound and inform player about wrong path
                        }
                        _unitMoved = true;
                    }
                }
            }
            if (!newCurrentUnit && !_unitMoved)
            {
                _gameInterface->HideUnitMenu();
                newCurrentUnit = _match->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit);
                _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
            }
        }
    }
    else if (_currentUnit && _currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        if (_currentUnit == newCurrentUnit)
        {
            if (_needToOpenMenuOnNextTapToSameUnit)
            {
                _gameInterface->ShowMenuForCurrentUni(this);
                _unitMenuShowed = true;
                _needToOpenMenuOnNextTapToSameUnit = false;
            }
            else
            {
                _gameInterface->HideUnitMenu();
                newCurrentUnit = NULL; // deselect current unit
                _removeFromLock = true;
            }
        }
    }
    if (!_unitMoved && ! _unitMenuShowed)
    {
        if (newCurrentUnit && _currentUnit != newCurrentUnit)
        {
            if (_currentUnit)
            {
                _currentUnit->selectedGameObjectDelegate = NULL;
				HideUnitPath();
				HidePathMap();
                
                if (_currentUnit->GetPath().size() > 0)
				{
                    _currentUnit->ClearTempPath();
				}
            }
            _currentUnit = newCurrentUnit;
            _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
            _gameInterface->HideUnitMenu();
            _currentUnit->selectedGameObjectDelegate = this;
            if (!_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding && _currentUnit->_owner_w->GetIsCurrentPlayer())
            {
				ShowUnitPath(_currentUnit);

                CCPoint location = _currentUnit->GetUnitCell();
                UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
                Pathfinder* pf = _match->_pathfinder;
                pf->MakePathMap(location.x, location.y, unitMoveType, _currentUnit->_unitCurrentParameters->GetMoveBalance());
                //pf->DumpMap();
                ShowPathMap();
            }
            
            engine->SelectUnit(_currentUnit->GetUnitObject());
            _gameInterface->OnCurrentUnitChanged(newCurrentUnit, false);
        }
        
        if (!newCurrentUnit)
        {
            if (_currentUnit)
            {
				if (_currentUnit->GetPath().size() > 0 && !_removeFromLock)
				{
					_currentUnit->ClearPath();
				}
				else
				{
					_gameInterface->OnCurrentUnitChanged(NULL, _removeFromLock);
					engine->SelectUnit(NULL);
					_currentUnit->selectedGameObjectDelegate = NULL;
					_currentUnit = NULL;
					HidePathMap();
				}				
				HideUnitPath();
            }
        }
    }
    //if (_currentUnit) {
    //    engine->SetPathZoneLevel(OBJECT_LEVEL_OVERGROUND);
    //}
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
    if (_freezeCounter>0) {
        return;
    }
    if (_currentUnit && !_currentUnit->GetIsFreezed())
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

void MAXGame::RefreshCurrentUnitPath()
{
    CCPoint location = _currentUnit->GetUnitCell();
    UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
    Pathfinder* pf = _match->_pathfinder;
    pf->MakePathMap(location.x, location.y, unitMoveType, _currentUnit->_unitCurrentParameters->GetMoveBalance());
    //pf->DumpMap();
    ShowPathMap();
    ShowUnitPath(_currentUnit);
}

bool MAXGame::CheckIfNextCellOk(GameUnit* unit)
{
    bool result = true;
    
    PFWaveCell* cell = unit->GetNextPathCell();
    if (cell)
    {
        UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
        Pathfinder* pf = _match->_pathfinder;
        int pfCost = pf->GetMapCostAt(cell->x, cell->y, cell->direction, unitMoveType);
        if (cell->cost != pfCost)
        {
            result = false;
        }
		else if (!unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isPlane)
		{
			if (_match->IsHiddenUnitInPos(cell->x, cell->y, false))
			{
				result = false;
			}
		}
    }
    
    return result;
}

void MAXGame::RecalculateUnitPath(GameUnit* unit)
{
    CCPoint location = unit->GetUnitCell();
    UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
    Pathfinder* pf = _match->_pathfinder;
    pf->MakePathMap(location.x, location.y, unitMoveType, unit->_unitCurrentParameters->GetMoveBalance());
    //pf->DumpMap();
    ShowPathMap();
    
    //HidePathMap();
    PFWaveCell* cell = unit->GetPath()[0];
    std::vector<PFWaveCell*> path = pf->FindPathOnMap(cell->x, cell->y); // call after MakePathMap
    if (path.size() > 1)
    {
        unit->SetPath(path);
        ShowUnitPath(unit);
    }
    else
    {
        unit->ClearPath();
		HideUnitPath();
    }
}

#pragma mark - SelectedGameObjectDelegate

void MAXGame::onUnitMoveStart(GameUnit* unit)
{
    _freezeCounter++;
    HidePathMap();
}

void MAXGame::onUnitMovePause(GameUnit* unit)
{
    RefreshCurrentUnitPath();
    if (unit == _currentUnit)
    {
        _freezeCounter--;
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit);
    }
}

void MAXGame::onUnitMoveStepBegin(GameUnit* unit)
{
    if (unit == _currentUnit)
    {
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit);
    }
}

void MAXGame::onUnitMoveStepEnd(GameUnit* unit)
{
    // check if unit path changed
    if (!CheckIfNextCellOk(unit))
    {
        unit->AbortCurrentPath();
        RecalculateUnitPath(unit);
    }
}

void MAXGame::onUnitMoveStop(GameUnit* unit)
{
    RefreshCurrentUnitPath();
    if (unit == _currentUnit)
    {
        _freezeCounter--;
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit);
    }

	// check landing pad
	if (unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isPlane)
	{
		CCPoint location = unit->GetUnitCell();
		USimpleContainer<GameUnit*> *units = _match->_agregator->UnitsInCell(location.x, location.y);
		for (int i = 0; i < units->GetCount(); i++)
		{
			GameUnit* u = units->objectAtIndex(i);
			if (u->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isLanding)
			{
				unit->Landing();
				break;
			}
		}
	}
}

void MAXGame::onUnitFireStart(GameUnit* unit)
{
    _freezeCounter++;
    HidePathMap();

    RefreshCurrentUnitPath();
    if (unit == _currentUnit)
    {
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit);
    }
}

void MAXGame::onUnitFireStop(GameUnit* unit)
{
    _freezeCounter--;
    RefreshCurrentUnitPath();

    // Do here damage calculation?
    // yes please
}

#pragma mark - GIUnitActionMenuDelegate

void MAXGame::OnUnitMenuItemSelected(UNIT_MENU_ACTION action)
{
	if (!_currentUnit)
	{
		return;
	}
	
    _needToOpenMenuOnNextTapToSameUnit = true;
    _gameInterface->HideUnitMenu();
    if (action == UNIT_MENU_ACTION_DONE)
	{
		_currentUnit->ConfirmCurrentPath();
		HideUnitPath();
        return;
	}
	else if (action == UNIT_MENU_ACTION_STOP)
	{
		if (_currentUnit->CanStartBuildProcess())
		{
			_currentUnit->StartBuildProcess();
		}
		else
		{
			_currentUnit->ClearPath();
			HideUnitPath();
		}
		return;
	}
	else if (action == UNIT_MENU_ACTION_START)
	{
		_currentUnit->StartBuildProcess();
		return;
	}
    bool _interfaceAction = false;
    switch (action) {
		case UNIT_MENU_ACTION_INFO:
        case UNIT_MENU_ACTION_ALLOCATE:
        case UNIT_MENU_ACTION_BUILD:
        case UNIT_MENU_ACTION_BUYUPGRADES:
        case UNIT_MENU_ACTION_REMOVE:
        case UNIT_MENU_ACTION_RESEARCH:
        case UNIT_MENU_ACTION_ACTIVATE:
            _interfaceAction = true;
            break;
            
        default:
            break;
    }
    
    if (_interfaceAction) {
        //show suitable screen
        return;
    }
    bool _needTargetUnit = false;
    switch (action) {
        case UNIT_MENU_ACTION_ATTACK:
        case UNIT_MENU_ACTION_DISABLE:
        case UNIT_MENU_ACTION_ENTER:
        case UNIT_MENU_ACTION_LOAD:
        case UNIT_MENU_ACTION_RECHARGE:
        case UNIT_MENU_ACTION_REPAIR:
        case UNIT_MENU_ACTION_RELOAD:
        case UNIT_MENU_ACTION_STEAL:
        case UNIT_MENU_ACTION_XFER:
            _needTargetUnit = false;
            break;
            
        default:
            break;
    }
    
    if (_needTargetUnit) {
        //select suitable target unit and perform action
        return;
    }
    //UNIT_MENU_ACTION_UPGRADE
    //UNIT_MENU_ACTION_STOP
    //UNIT_MENU_ACTION_START
    //UNIT_MENU_ACTION_SENTRY
    
    
    //UNIT_MENU_ACTION_XFORM
    //UNIT_MENU_ACTION_FOLLOW
    //UNIT_MENU_ACTION_PLACE
    //UNIT_MENU_ACTION_WAIT
}

#pragma mark - MAXAnimationDelegate

void MAXGame::OnAnimationStart(MAXAnimationBase* animation)
{}

void MAXGame::OnAnimationUpdate(MAXAnimationBase* animation)
{}

void MAXGame::OnAnimationFinish(MAXAnimationBase* animation)
{
    if (animation == _waitTestAnimCorvette)
    {
        _waitTestAnimCorvette = NULL;
        _waitTestAnimCorvetteMovement = moveUnit(_testUnitCorvette, 1.5);
    }
    if (animation == _waitTestAnimCorvetteMovement)
    {
        _waitTestAnimCorvetteMovement = NULL;
        _waitTestAnimSubmarine = prepareUnitToMoveToPoint(_testUnitSubmarine, ccp(28, 41), 1.5);
    }
    if (animation == _waitTestAnimSubmarine)
    {
        _waitTestAnimCorvette = NULL;
        _waitTestAnimSubmarineMovement = moveUnit(_testUnitSubmarine, 2);
    }
    if (animation == _waitTestAnimSubmarineMovement) {
        _waitTestAnimSubmarineMovement = NULL;
        _freezeCounter --;
        
        _testUnitSubmarine = NULL;
        _testUnitCorvette = NULL;
    }
}

