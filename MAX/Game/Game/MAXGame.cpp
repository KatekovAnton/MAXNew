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

MAXGame::MAXGame()
:_testUnit(NULL), iteration(0), _pathVisualizer(NULL), _freezeCounter(0)
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
//    StartTest();
}

void MAXGame::StartTest()
{
    if (!_testUnit) {
        return;
    }
    _testUnit->SetDirection(3);

    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    CCPoint _unitCell = _testUnit->GetUnitCell();
    for (int i = 0; i < 30; i++) {
        CCPoint destination = ccp(_unitCell.x + 1, _unitCell.y + 1);
        MAXAnimationObjectUnit* step2 = new MAXAnimationObjectUnit(_unitCell ,destination, _testUnit->GetUnitObject(), MAXANIMATION_CURVE_EASE_LINEAR);
        _unitCell = destination;
        step2->_delegate = _testUnit;
        sequence->AddAnimation(step2);
    }
    MAXAnimationObjectUnit* step1 = new MAXAnimationObjectUnit(_testUnit->GetUnitObject()->GetBodyIndex(), 7, _testUnit->GetUnitObject()->GetPureHeadIndex(), _testUnit->GetUnitObject());
    sequence->AddAnimation(step1);
    for (int i = 0; i < 5; i++) {
        CCPoint destination = ccp(_unitCell.x - 1, _unitCell.y - 1);
        MAXAnimationObjectUnit* step2 = new MAXAnimationObjectUnit(_unitCell ,destination, _testUnit->GetUnitObject(), MAXANIMATION_CURVE_EASE_LINEAR);
        _unitCell = destination;
        step2->_delegate = _testUnit;
        sequence->AddAnimation(step2);
    }
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
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
    for (int x = 0; x < _match->_map->GetMapWidth(); x++)
    {
        for (int y = 0; y < _match->_map->GetMapHeight(); y++)
        {
            // remove highlight at x, y
        }
    }
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
    
    
    
    
    _gameInterface = new GameInterface();
    _gameInterface->InitBaseInterface();
    CCDirector::sharedDirector()->pushScene(_gameInterface);
//    for (int x = 0; x < 100; x++)
//    {
//        for (int y = 0; y < 100; y++)
//        {
//            GameUnit *unit1 = _match->_players[0]->CreateUnit(x, y, "Inter", 0);
//            unit1->SetRandomDirection();
//            unit1->PlaceUnitOnMap();
//        }
//    }
//    return;
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
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 57, "Aagunm", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(57, 58, "Awac", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
        _testUnit = unit1;
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
        for (int i = 55; i <= 62; i++) {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(i, 43, "Road", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    {
        for (int i = 28; i <= 34; i++) {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(i, 32, "Plat", 0);
            unit1->PlaceUnitOnMap();
        }
        for (int i = 41; i <= 47; i++) {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(i, 32, "Plat", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    {
        for (int i = 35; i <= 40; i++) {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(i, 32, "Bridge", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(50, 64, "Scanner", 0);
        unit1->SetRandomDirection();
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[1]->CreateUnit(50, 56, "Inter", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[1]->CreateUnit(50, 53, "Inter", 0);
        unit1->PlaceUnitOnMap();
    }
     
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(68, 45, "Airplant", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(70, 45, "gref", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(70, 43, "Powerpl", 0);
        unit1->PlaceUnitOnMap();
        unit1->StartBuildProcess();
    }
    {
        _match->_players[0]->CreateUnit(72, 43, "Goldstore", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 44, "landpad", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 45, "Matstore", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 46, "Powergen", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(72, 47, "Fuelstore", 0)->PlaceUnitOnMap();
    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(68, 47, "Conn", 0);
        unit1->PlaceUnitOnMap();
        unit1 = _match->_players[0]->CreateUnit(68, 48, "Conn", 0);
        unit1->PlaceUnitOnMap();
        for (int i = 68; i < 79; i++) {
            unit1 = _match->_players[0]->CreateUnit(i, 42, "Conn", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    {
        for (int i = 60; i < 70; i++) {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(i, 51, "Conblock", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(68, 43, "Hvplant", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(65, 43, "Hang", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(63, 43, "Powerpl", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(80, 41, "Shipyard", 0);
        unit1->PlaceUnitOnMap();
        vector<CCPoint> points = unit1->GetNerbyCells();
        for (int i = 0; i < points.size(); i++) {
            unit1 = _match->_players[0]->CreateUnit(points[i].x, points[i].y, "Conn", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(80, 38, "Dock", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(65, 42, "Radar", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(66, 42, "turret", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(64, 42, "Zenit", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(63, 42, "Arturret", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(62, 42, "misturret", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        for (int i = 44; i < 49; i++) {
            GameUnit *unit1 = _match->_players[0]->CreateUnit(67, i, "Conn", 0);
            unit1->PlaceUnitOnMap();
        }
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(68, 49, "Lightplant", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(66, 49, "barrak", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(64, 49, "Ecosphere", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(62, 49, "Habitat", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(60, 49, "Store", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(65, 46, "Mining", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(63, 46, "research", 0);
        unit1->PlaceUnitOnMap();
    }
    {
        GameUnit *unit1 = _match->_players[0]->CreateUnit(61, 46, "pehplant", 0);
        unit1->PlaceUnitOnMap();
    }
    
    
    
    engine->SetCameraCenter(ccp(62, 49));

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

#pragma mark - Interface
#pragma mark Messages
void MAXGame::ShowUnitSpottedMessage(GameUnit* unit)
{
    printf("Enemy %s spotted at %d, %d!\n", unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_name.c_str(), (int)unit->GetUnitCell().x, (int)unit->GetUnitCell().y);
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
}

void MAXGame::ProceedPan(float speedx, float speedy)
{
    engine->MoveCamera(speedx, speedy);
}

void MAXGame::ShowUnitPath(GameUnit *unit)
{
    std::vector<PFWaveCell*> path = unit->GetPath();
    int pathStep = unit->GetPathIndex();
    int pathSize = path.size();
    if (pathStep > pathSize - 2)
        pathStep = pathSize - 2;
    int speed = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed * 10; // rework to current data
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
            if (totalCost + path[pi - 1]->cost > speed)
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
            totalCost -= speed;
            speed = unit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed * 10; // rework to max param
            element.image += 8;
        }
		testPath.push_back(element);
	}
	_pathVisualizer->VisualizePath(testPath);
}

void MAXGame::ProceedTap(float tapx, float tapy)
{
    if (_freezeCounter>0) {
        return;
    }
    bool _unitMoved = false;
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    
 //   printf("(%d, %d) = res=%d, scan=%d\n", (int)p.x, (int)p.y, _match->_currentPlayer_w->_resourceMapFog->GetValue(p), _match->_currentPlayer_w->_fog->GetValue(p));
    bool _removeFromLock = false;
    
    GameUnit* newCurrentUnit = _match->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit != NULL);// _currentPlayer_w->GetUnitInPosition(p);
    if (_currentUnit && !_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        if (p.x < 0 || p.x>= _match->_map->GetMapWidth() || p.y < 0 || p.y >= _match->_map->GetMapHeight())
        {}
        else if (_currentUnit == newCurrentUnit)
        {
            newCurrentUnit = NULL; // deselect current unit
            _removeFromLock = true;
        }
        else
        {
            if (newCurrentUnit)
            {
                // force select another unit
            }
            else if (_match->_currentPlayer_w == _currentUnit->_owner_w)
            {
                if (!_currentUnit->GetIsFreezed())
                {
                    if (_currentUnit->IsPathTargetedTo(p.x, p.y))
                    {
                        _currentUnit->ConfirmCurrentPath();
                        _pathVisualizer->Clear();
						HidePathMap();
                        _unitMoved = true;
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
                newCurrentUnit = _match->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit != NULL);
            }
        }
    }
    else if (_currentUnit && _currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        if (_currentUnit == newCurrentUnit)
        {
            newCurrentUnit = NULL; // deselect current unit
            _removeFromLock = true;
        }
    }
    if (!_unitMoved)
    {
        if (newCurrentUnit && _currentUnit != newCurrentUnit)
        {
            if (_currentUnit)
            {
                _currentUnit->selectedGameObjectDelegate = NULL;
				_pathVisualizer->Clear();
				HidePathMap();
                
                if (_currentUnit->GetPath().size() > 0)
				{
                    _currentUnit->ClearTempPath();
				}
            }
            _currentUnit = newCurrentUnit;
            _currentUnit->selectedGameObjectDelegate = this;
            if (!_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding && _currentUnit->_owner_w->GetIsCurrentPlayer())
            {
				ShowUnitPath(_currentUnit);

                CCPoint location = _currentUnit->GetUnitCell();
                UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
                Pathfinder* pf = _match->_pathfinder;
                pf->MakePathMap(location.x, location.y, unitMoveType, _currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed * 10);
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
				if (_currentUnit->GetPath().size() > 0)
				{
					std::vector<PFWaveCell*> path;
					_currentUnit->SetPath(path);
				}
				else
				{
					_gameInterface->OnCurrentUnitChanged(NULL, _removeFromLock);
					engine->SelectUnit(NULL);
					_currentUnit->selectedGameObjectDelegate = NULL;
					_currentUnit = NULL;
					HidePathMap();
				}				
				_pathVisualizer->Clear();
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
            GameEffect* effect = _currentUnit->Fire(p);
            if (effect)
            {
                effect->_delegate_w = this;
                effect->_tag = GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER;
                engine->hidePathZoneCounter ++;
                _freezeCounter ++;
            }
            
        }
        
    }
}

#pragma mark - SelectedGameObjectDelegate

void MAXGame::onUnitStartMove(GameUnit* unit)
{
    if (unit == _currentUnit)
    {
        HidePathMap();
    }
}

void MAXGame::onUnitStopMove(GameUnit* unit)
{
    if (unit == _currentUnit)
    {
        CCPoint location = _currentUnit->GetUnitCell();
        UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)_currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bMoveType;
        Pathfinder* pf = _match->_pathfinder;
        pf->MakePathMap(location.x, location.y, unitMoveType, _currentUnit->_unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed * 10);
        //pf->DumpMap();
        ShowPathMap();
        ShowUnitPath(_currentUnit);
    }
}

#pragma mark - GameEffectDelegate

void MAXGame::GameEffectDidFinishExistance(GameEffect* effect)
{
    if (effect->_tag == GAME_OBJECT_TAG_FIRE_OBJECT_CONTROLLER) {
        engine->hidePathZoneCounter --;
        _freezeCounter --;
    }
}





