//
//  MAXGameController.cpp
//  MAX
//
//  Created by Anton Katekov on 20.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXGameController.h"
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

#include "MAXGameInputController.h"

#include "MatchMapAgregator.h"

#include "GameMap.h"
#include "GameFog.h"
#include "GameUnit.h"
#include "GameMatch.h"
#include "GameEffect.h"
#include "GameInteface.h"
#include "GameUnitParameters.h"
#include "GameUnitData.h"
#include "GamePathVisualizer.h"

#include "Pathfinder.h"
#include "PFWaveCell.h"

#include "GIMessageWindow.h"

static bool allowControlEnemyUnits = false;

MAXGameController::MAXGameController()
	:_pathVisualizer(NULL), _freezeCounter1(0), _fireDelayAnim(NULL), _endDelayAnim(NULL), _iputController(new MAXGameInputController()), _currentTargetUnit(NULL), _startAttackModeAgain(false), _windowCloseQuestion(NULL)
{
	_iputController->_delegate_w = this;
    _pathVisualizer = new GamePathVisualizer();
}

MAXGameController::~MAXGameController()
{
	DeselectCurrentUnit(true);
	
	CCDirector::sharedDirector()->popScene();
	_gameInterface->release();
	_gameInterface = NULL;
	game->SetLoadingProgress(0.0);
    delete _iputController;
	game->SetLoadingProgress(0.1);
	delete _match;
	game->SetLoadingProgress(0.7);
	engine->_delegate = NULL;
    delete _pathVisualizer;
	game->SetLoadingProgress(1.0);
}

void MAXGameController::Init()
{
    engine->_delegate = this;
    SOUND->SetHoldEffects(true);
    StartMatch();
    SOUND->SetHoldEffects(false);
}

void MAXGameController::EndMatch()
{
	if (_freezeCounter1 != 0)
		return;

	MAXAnimationWait *wait = new MAXAnimationWait(interfaceAnimationTime * 3);
	wait->_delegate = this;
	_endDelayAnim = wait;
	MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);

}

void MAXGameController::OnOptionsPressed()
{
	if (_freezeCounter1 != 0)
		return;
    
    vector<string> buttons;
    buttons.push_back("Yes");
    buttons.push_back("No");
    GIMessageWindow *window = new GIMessageWindow("", "Do you really want to exit?", buttons);
    window->autorelease();
    window->_delegate_w = this;
    _gameInterface->PresentWindow(window);
    _windowCloseQuestion = window;
}

void MAXGameController::DeletionProgressDidChange(float zeroToOne)
{
	game->SetLoadingProgress(zeroToOne * 0.4 + 0.2);
}

void MAXGameController::IncreaseFreezeCounter()
{
    if (_freezeCounter1 == 0) {
        HidePathMap();
        HideUnitPath();
    }
    _freezeCounter1 ++;
    if (_freezeCounter1 == 1) 
        _iputController->OnGameStartsActons();
    
}

void MAXGameController::DecreaseFreezeCounter()
{
    _freezeCounter1 --;
    if (_freezeCounter1 == 0)
    {
		
		RefreshCurrentUnitPath();
      //  if (_gameController->GetRunedSpecialAction())
            _iputController->OnGameStopsActons();
        
        if (_iputController->UnitCanMoveWithAction())
        {
            if (_currentUnit) {
                if (_currentUnit->_owner_w->GetIsCurrentPlayer()) {
                    ShowUnitPath(_currentUnit);
                    
                    RecalculateUnitPathMap(_currentUnit);
                    ShowPathMap();
                }
            }
        }
    }
}

int MAXGameController::CurrentPlayerId() const
{
    return _match->_currentPlayer_w->GetPlayerId();
}

void MAXGameController::StartMatch()
{
	
    game->SetLoadingProgress(0.05);
    MAXConfigManager::SharedMAXConfigManager()->LoadConfigsFromFile("UnitListOriginal.txt");
    game->SetLoadingProgress(0.3);
    MAXConfigManager::SharedMAXConfigManager()->LoadClanConfigsFromFile("clansOriginal.cfg");
    game->SetLoadingProgress(0.4);

    vector<GameMatchPlayerInfo> infos;
    GameMatchPlayerInfo player1 = {0, 3, "Test player1", {255,0,0,255}};
    infos.push_back(player1);
    
    GameMatchPlayerInfo player2 = {1, 0, "Test player2", {0,255,0,255}};
    infos.push_back(player2);
    
//    GameMatchPlayerInfo player3 = {2, 4, "Test player3", {0,0,255,255}};
//    infos.push_back(player3);
    _match = new GameMatch("Green_6.wrl", infos);
	_match->_gameController = this;
    _currentUnit = NULL;
    game->SetLoadingProgress(0.75);
    
    
    
    _gameInterface = new GameInterface();
	_gameInterface->_gameController = this;
    _gameInterface->InitBaseInterface();
    game->SetLoadingProgress(0.9);

    _match->_holdAutofire = true;
    {
        _match->_players[0]->CreateUnit(55, 37, "Road", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(55, 37, "Engineer", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(56, 56, "Inter", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(56, 49, "Constructor", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(42, 32, "Constructor", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(45, 47, "Corvette", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(47, 49, "Escort", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(44, 49, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(43, 41, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(43, 45, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(44, 44, "sub", 0)->PlaceUnitOnMap();
    	_match->_players[0]->CreateUnit(45, 45, "Seacargo", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(46, 45, "Seafuel", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(45, 46, "Rokcr", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(44, 42, "seaminelay", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(30, 34, "Scout", 0)->PlaceUnitOnMap();

		_match->_players[0]->CreateUnit(45, 43, "Seatrans", 0)->PlaceUnitOnMap();
	    _match->_players[0]->CreateUnit(78, 45, "pcan", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(57, 52, "Bomber", 0)->PlaceUnitOnMap();
    	_match->_players[0]->CreateUnit(58, 52, "Airtrans", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(57, 53, "Crawler", 0)->PlaceUnitOnMap();
	    _match->_players[0]->CreateUnit(57, 51, "infantry", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(55, 51, "Infil", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(57, 57, "Aagunm", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(57, 58, "Awac", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(58, 58, "Scout", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(48, 58, "Engineer", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(48, 59, "Conn", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(48, 60, "Conn", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(59, 53, "Asgun", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(56, 54, "Inter", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(55, 52, "Tank", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(53, 54, "Rocket", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(56, 58, "Inter", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(55, 58, "Surveyor", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(65, 58, "Dozer", 0)->PlaceUnitOnMap();
    	_match->_players[0]->CreateUnit(65, 59, "Fueltruck", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(66, 59, "GTruck", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(67, 59, "Truck", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(66, 60, "Repair", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(73, 52, "Miner", 0)->PlaceUnitOnMap();
	    _match->_players[0]->CreateUnit(66, 58, "alnasgun", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(66, 57, "alnplane", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(64, 55, "alntank", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(44, 33, "juger", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(50, 64, "Scanner", 0)->PlaceUnitOnMap();
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
			_match->_players[0]->CreateUnit(36 + ((i / 2) % 2), i + 3, "seamine", 0)->PlaceUnitOnMap();
		for (int i = 55; i <= 62; i++)
            _match->_players[0]->CreateUnit(i, 43, "Road", 0)->PlaceUnitOnMap();
        for (int i = 28; i <= 34; i++)
            _match->_players[0]->CreateUnit(i, 32, "Plat", 0)->PlaceUnitOnMap();
        for (int i = 41; i <= 47; i++)
            _match->_players[0]->CreateUnit(i, 32, "Plat", 0)->PlaceUnitOnMap();
        for (int i = 41; i <= 47; i++)
            _match->_players[0]->CreateUnit(i, 31, "Plat", 0)->PlaceUnitOnMap();
        for (int i = 35; i <= 40; i++)
            _match->_players[0]->CreateUnit(i, 32, "Bridge", 0)->PlaceUnitOnMap();
        for (int i = 44; i < 49; i++)
            _match->_players[0]->CreateUnit(67, i, "Conn", 0)->PlaceUnitOnMap();
        
        _match->_players[0]->CreateUnit(48, 31, "Constructor", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(48, 32, "Engineer", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(48, 33, "Rocket", 0)->PlaceUnitOnMap();
		_match->_players[0]->CreateUnit(48, 34, "Scout", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(70, 43, "Powerpl", 0)->PlaceUnitOnMap();
        GameUnit *unit1 = _match->_players[0]->CreateUnit(80, 41, "Shipyard", 0);
        unit1->PlaceUnitOnMap();
        vector<CCPoint> points = unit1->GetNerbyCells();
        for (int i = 0; i < points.size(); i++) 
            _match->_players[0]->CreateUnit(points[i].x, points[i].y, "Conn", 0)->PlaceUnitOnMap();
    
        
        _match->_players[0]->CreateUnit(30, 44, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(31, 43, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(31, 44, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(31, 45, "sub", 0)->PlaceUnitOnMap();
        _match->_players[0]->CreateUnit(32, 40, "sub", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(25, 58, "Surveyor", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(30, 43, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(36, 37, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(30, 45, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(29, 45, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(24, 42, "Corvette", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(24, 43, "Corvette", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(24, 44, "Corvette", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(24, 45, "Corvette", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(24, 46, "Corvette", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(23, 42, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(23, 43, "Escort", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(23, 44, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(23, 45, "Escort", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(23, 46, "Gunboat", 0)->PlaceUnitOnMap();
		
        _match->_players[1]->CreateUnit(22, 41, "Gunboat", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(22, 42, "Seacargo", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(22, 43, "Rokcr", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(22, 44, "Awac", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(35, 34, "Awac", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(22, 45, "Rokcr", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(22, 46, "Seacargo", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(22, 47, "Gunboat", 0)->PlaceUnitOnMap();
 
        _match->_players[1]->CreateUnit(39, 55, "Inter", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(36, 53, "Inter", 0)->PlaceUnitOnMap();
//        for (int i = 61; i < 69; i++)
//            _match->_players[1]->CreateUnit(i, 56, "landmine", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(50, 47, "pcan", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(52, 48, "Infil", 0)->PlaceUnitOnMap();
        _match->_players[1]->CreateUnit(42, 42, "sub", 0)->PlaceUnitOnMap();
		for (int i = 32; i < 40; i+=2)
			_match->_players[1]->CreateUnit(i, 31, "seamine", 0)->PlaceUnitOnMap();
        
        
   // _match->_players[2]->CreateUnit(43, 52, "scout", 0)->PlaceUnitOnMap();
	}
	
    _match->_players[0]->_playerData->cameraPosition = ccp(46, 41);
    _match->_players[1]->_playerData->cameraPosition = ccp(47, 43);
  //  _match->_players[2]->_playerData->cameraPosition = ccp(44, 45);
    
    
    engine->SetCameraCenter(_match->_players[0]->_playerData->cameraPosition);
    engine->SetZoom(_match->_players[0]->_playerData->cameraZoom);
    
    _match->_holdAutofire = false;
	
    game->SetLoadingProgress(1.0);
	CCDirector::sharedDirector()->pushScene(_gameInterface);
	engine->FinishLoading();
    
    _gameInterface->SetNewTurnData(_match->_currentTurn, _match->_currentPlayer_w->_playerData->_playerInfo._color);
}

bool MAXGameController::EndTurn()
{
    bool result = false;
    if (_freezeCounter1 != 0)
        return false;
    _iputController->AbortCurrentAction();
    _match->_currentPlayer_w->_playerData->cameraPosition = engine->ScreenToWorldCell( _gameInterface->GetCenter());
    _match->_currentPlayer_w->_playerData->cameraZoom = engine->CameraZoom();
    result = _match->EndTurn();
    if (!result) 
        return result;
    
    _gameInterface->SetLockUnits(false);
    _gameInterface->HideUnitMenu();
    _gameInterface->HideUnitSelectionMenu();
    _gameInterface->ClearLockedUnits();
    
	DeselectCurrentUnit(true);
    _gameInterface->SetNewTurnData(_match->_currentTurn, _match->_currentPlayer_w->_playerData->_playerInfo._color);
    
    engine->SetZoom(_match->_currentPlayer_w->_playerData->cameraZoom);
    engine->SetCameraCenter(_match->_currentPlayer_w->_playerData->cameraPosition);
    return result;
}

bool MAXGameController::EscapeStealthUnitFromPos(GameUnit* unit, const int x, const int y, GameMatchPlayer *reasonPlayer, vector<CCPoint> lockedCells)
{
    bool result = false;
    MAXObjectConfig* config = unit->_unitData->GetConfig();
    if (config->_isBuilding)
    {
        if (config->_isBombMine)
        {
            result = true; // bomb always escaped! Unit - welcomme to bomb cell!
        }
    }
    else
    {
        int startDirection = nextIntMax(8);
        int direction = startDirection;
        Pathfinder* pf = unit->_owner_w->_pathfinder;
        UNIT_MOVETYPE movetype = (UNIT_MOVETYPE)config->_bMoveType;
        for (;;)
        {
            int newX = x + PFWaveCell::DirToDX(direction);
            int newY = y + PFWaveCell::DirToDY(direction);
            int cost = pf->GetMapCostAt(newX, newY, direction, movetype);
            bool available = true;
            for (int i = 0; i < lockedCells.size(); i++)
            {
                CCPoint lp = lockedCells[i];
                if ((int)lp.x == newX && (int)lp.y == newY)
                {
                    available = false;
                    break;
                }
            }
            if ((cost >= 0) && (cost <= unit->_unitData->GetMoveBalance()) && available)
            {
                if (!_match->IsHiddenUnitInPos(newX, newY, true, reasonPlayer, lockedCells))
                {
                    unit->EscapeToLocation(newX, newY, cost);
                    result = true;
                    break;
                }
            }
            
            direction++;
            if (direction >= 8)
                direction -= 8;
            
            if (direction == startDirection)
                break;
        }
    }
    return result;
}

#pragma mark - Path map

void MAXGameController::ShowPathMap()
{
    HidePathMap();
    Pathfinder* pf = _match->_currentPlayer_w->_pathfinder;
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

void MAXGameController::HidePathMap()
{
	engine->ClearPathZone();
}

void MAXGameController::RefreshCurrentUnitPath()
{
	if (!_currentUnit)
		return;
    CCPoint location = _currentUnit->GetUnitCell();
    UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)_currentUnit->_unitData->GetConfig()->_bMoveType;
    _currentUnit->_owner_w->_pathfinder->MakePathMap(location.x, location.y, unitMoveType, _currentUnit->_unitData->GetMoveBalance());
    //pf->DumpMap();
	if (_iputController->UnitCanMoveWithAction())
	{
		ShowPathMap();
		ShowUnitPath(_currentUnit);
	}
}

bool MAXGameController::CheckIfNextCellOk(GameUnit* unit)
{
    bool result = true;
    
    PFWaveCell* cell = unit->GetNextPathCell();
    if (cell)
    {
        UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)unit->_unitData->GetConfig()->_bMoveType;
        Pathfinder* pf = unit->_owner_w->_pathfinder;
        int pfCost = pf->GetMapCostAt(cell->x, cell->y, cell->direction, unitMoveType);
        if (cell->cost != pfCost)
            result = false;
        
		else if (!unit->_unitData->GetConfig()->_isPlane)
		{
            vector<CCPoint> empty;
			if (_match->IsHiddenUnitInPos(cell->x, cell->y, false, unit->_owner_w, empty))
				result = false;
			
		}
    }
    
    return result;
}

void MAXGameController::RecalculateUnitPathMap(GameUnit *unit)
{
    CCPoint location = unit->GetUnitCell();
    UNIT_MOVETYPE unitMoveType = (UNIT_MOVETYPE)unit->_unitData->GetConfig()->_bMoveType;
    unit->_owner_w->_pathfinder->MakePathMap(location.x, location.y, unitMoveType, unit->_unitData->GetMoveBalance());
}

#pragma mark - Unit path

void MAXGameController::ShowUnitPath(GameUnit *unit)
{
    HideUnitPath();
    std::vector<PFWaveCell*> path = unit->GetPath();
    int pathStep = unit->GetPathIndex();
    int pathSize = path.size();
    if (pathStep > pathSize - 2)
        pathStep = pathSize - 2;
    int speed = unit->_unitData->GetMoveBalance();
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
			speed = unit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_SPEED);
		}
		testPath.push_back(element);
	}
	_pathVisualizer->VisualizePath(testPath);
}

void MAXGameController::HideUnitPath()
{
    _pathVisualizer->Clear();
}

void MAXGameController::UpdateCurrentUnitPath()
{
    if (_currentUnit)
    {
		if (_currentUnit->CanMove() && _iputController->UnitCanMoveWithAction()) {
            ShowPathMap();
            ShowUnitPath(_currentUnit);
        }
        else
        {
            _currentUnit->ClearPath();
            _currentUnit->ClearTempPath();
            HidePathMap();
            HideUnitPath();
        }
    }
    else
    {
        HidePathMap();
        HideUnitPath();
    }
}

void MAXGameController::RecalculateUnitPath(GameUnit* unit)
{
    RecalculateUnitPathMap(unit);
    ShowPathMap();
    
    if (unit->GetPath().size() == 0) 
        return;
    
    PFWaveCell* cell = unit->GetPath()[0];
    Pathfinder* pf = unit->_owner_w->_pathfinder;
    std::vector<PFWaveCell*> path = pf->FindPathOnMap(cell->x, cell->y);
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

#pragma mark - Interface

void MAXGameController::ShowUnitSpottedMessage(GameUnit* unit)
{
    printf("Enemy %s spotted at %d, %d!\n", unit->_unitData->GetConfig()->_name.c_str(), (int)unit->GetUnitCell().x, (int)unit->GetUnitCell().y);
    _gameInterface->ShowUnitSpottedMessage(unit);
}

#pragma mark - MAXGameControllerDelegate

void MAXGameController::SelectLargeBuildingConstructionPlaceActionFinished(CCPoint result, MAXObjectConfig *buildingConfig)
{
    vector<CCPoint> points;
    points.push_back(result);
    if (buildingConfig->_bSize == 2) {
        points.push_back(ccp(result.x+1, result.y));
        points.push_back(ccp(result.x, result.y+1));
        points.push_back(ccp(result.x+1, result.y+1));
    }
    
    bool start = true;
    for (int i = 0; i < points.size(); i++) {
        CCPoint point = points[i];
        if (_match->IsHiddenUnitInPos(point.x, point.y, false, _match->_currentPlayer_w, points)) {
            start = false;
            break;
        }
    }
    if (start)
        _currentUnit->StartConstructingUnitInPlace(result, buildingConfig);
    else
        _currentUnit->AbortConstructingUnit();
    UpdateCurrentUnitPath();
    _gameInterface->HideUnitMenu();
}

void MAXGameController::SelectLargeBuildingConstructionPlaceActionCanceled()
{
    RecalculateUnitPathMap(_currentUnit);
    ShowPathMap();
}

void MAXGameController::SelectSmallBuildingConstructionPathActionFinished(CCPoint result, MAXObjectConfig *buildingConfig)
{
    _currentUnit->StartConstructingUnitInPlace(result, buildingConfig);
    UpdateCurrentUnitPath();
    _gameInterface->HideUnitMenu();
}

void MAXGameController::SelectSecondUnitActionCanceled()
{
    _startAttackModeAgain = false;
    if (_currentUnit) {
        RecalculateUnitPathMap(_currentUnit);
        ShowPathMap();
    }
}

bool MAXGameController::CanExitToCell(CCPoint cell, vector<CCPoint> lockedCells)
{
    return !_match->IsHiddenUnitInPos(cell.x, cell.y, false, _match->_currentPlayer_w, lockedCells);
}

GameMatch *MAXGameController::GetCurrentMatch()
{
	return _match;
}

void MAXGameController::SelectSecondUnitActionFinished(const vector<GameUnit*> units, const CCPoint &cellPoint, UNIT_MENU_ACTION action)
{
    _startAttackModeAgain = true;
    
    switch (action) {
        case UNIT_MENU_ACTION_ATTACK:
        {
            if (units.size() == 0)
            {
                bool cannotFire = _currentUnit->_unitData->IsCellOfUnit(cellPoint);
                
                GROUND_TYPE type = _match->_map->GroundTypeAtXY(cellPoint.x, cellPoint.y);
                if (_currentUnit->GetConfig()->_pFireType == 2 && !(type == GROUND_TYPE_COAST || type == GROUND_TYPE_WATER))
                    cannotFire = true;
                
                if (cannotFire)
                {
                    ShowPathMap();
                    return;
                }
				vector<GameUnit*> units;
				units.push_back(_currentUnit);
				StartMultipleAttackSequence(units, NULL, cellPoint, true);
                _match->GameUnitDidInitiateFire(_currentUnit);
            }
            else
                _gameInterface->ShowUnitSelectionMenu(this, units, cellPoint);
        } break;
        case UNIT_MENU_ACTION_RELOAD:
        {
            if (units.size() != 0)
            {
                EnableModeForCurrentUnit(action);
                for (int i = 0; i < units.size(); i++)
                {
                    GameUnit *rearmedUnit = units[units.size() - i - 1];
                    if (rearmedUnit->_unitData->RearmWithUnit(_currentUnit->_unitData))
                    {
                        SOUND->PlaySystemSound(SOUND_TYPE_RELOADED);
                        SOUND->PlayGameSound(_currentUnit->GetConfig()->_soundWorkName, NULL, false, 1.0);
                        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
                        break;
                    }
                }
            }
        } break;
        case UNIT_MENU_ACTION_REPAIR:
        {
            if (units.size() != 0)
            {
                EnableModeForCurrentUnit(action);
                for (int i = 0; i < units.size(); i++)
                {
                    GameUnit *repairedUnit = units[units.size() - i - 1];
                    if (repairedUnit->_unitData->RepairWithUnit(_currentUnit->_unitData))
                    {
                        SOUND->PlaySystemSound(SOUND_TYPE_UNIT_REPAIRED);
                        SOUND->PlayGameSound(_currentUnit->GetConfig()->_soundWorkName, NULL, false, 1.0);
                        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
                        break;
                    }
                }
            }
        } break;
            
        default:
            break;
    }
}

#pragma mark - MAXEngineDelegate

void MAXGameController::onFrame()
{
    SoundEngine::sharedInstance()->CheckStoppedSound();
    for(int i = 0; i < _match->_players.size(); i++)
        _match->_players[i]->SetPalette(engine->FullTime());
}

#pragma mark - DisplayPinchDelegate

bool MAXGameController::CanStartPinch(float x, float y)
{    
    float _y = Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale() - y;
    return !_gameInterface->ShouldReceiveTouch(x, _y);
}

void MAXGameController::ProceedPinch(float scale)
{
    if (_iputController->ShoulTakePinch(scale)) {
        _iputController->ProceedPinch(scale);
        return;
    }
    engine->ScaleCamera(scale);
    if (_gameInterface->GetUnitMenuOpened()) {
        _gameInterface->HideUnitMenu();
        _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
    }
    if (_gameInterface->GetSelectUnitMenuOpened()) {
        _gameInterface->HideUnitSelectionMenu();
        EnableModeForCurrentUnit(UNIT_MENU_ACTION_ATTACK);
    }
}

void MAXGameController::ProceedPan(float speedx, float speedy)
{
    if (_iputController->ShoulTakePan()) {
        _iputController->ProceedPan(speedx, speedy);
        return;
    }
    engine->MoveCamera(speedx, speedy);
    if (_gameInterface->GetUnitMenuOpened()) {
        _gameInterface->HideUnitMenu();
        _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
    }
    
    if (_gameInterface->GetSelectUnitMenuOpened()) {
        _gameInterface->HideUnitSelectionMenu();
        EnableModeForCurrentUnit(UNIT_MENU_ACTION_ATTACK);
        return;
    }
}

void MAXGameController::ProceedTap(float tapx, float tapy)
{
    if (_freezeCounter1>0) {
        return;
    }
    if (_gameInterface->GetSelectUnitMenuOpened()) {
        _gameInterface->HideUnitSelectionMenu();
        EnableModeForCurrentUnit(UNIT_MENU_ACTION_ATTACK);
        return;
    }
    
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    
    printf("tap:%f %f \n", p.x, p.y);
    
    
    if (_iputController->ShoulTakeTap(p)) {
        _iputController->ProceedTap(tapx, tapy);
        if (_iputController->shouldDeselectUnit) {
            DeselectCurrentUnit(true);
        }
        return;
    }
    
    bool _unitMoved = false;
    bool _unitMenuShowed = false;
    bool _removeFromLock = false;
    
    
    GameUnit* newCurrentUnit = _match->_currentPlayer_w->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit, false);
    
    bool tapToSameUnit = false;
    if (_currentUnit)
        tapToSameUnit = _currentUnit->_unitData->IsCellOfUnit(p);
    
    if (_currentUnit && _currentUnit->CanMove())
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
            if (tapToSameUnit && _needToOpenMenuOnNextTapToSameUnit) {
                _gameInterface->ShowMenuForCurrentUni(this);
                _unitMenuShowed = true;
                _needToOpenMenuOnNextTapToSameUnit = false;
            }
            else if (newCurrentUnit)
            {
                if (_currentUnit->GetPath().size() > 0)
                {
                    CCPoint location1 = _currentUnit->GetUnitCell();
                    CCPoint location2 = newCurrentUnit->GetUnitCell();
                    if ((location1.x == location2.x) && (location1.y == location2.y))
                    {
                        // clear path
                        std::vector<PFWaveCell*> path;
                        _currentUnit->SetPath(path);
                        HideUnitPath();
                        _needToOpenMenuOnNextTapToSameUnit = false;
                    }
                }
                
                SelectNewUnit(newCurrentUnit);
            }
            else if (_currentUnit->_owner_w->GetIsCurrentPlayer() || allowControlEnemyUnits)
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
                        _needToOpenMenuOnNextTapToSameUnit = true;
                    }
                    else
                    {
                        CCPoint location = _currentUnit->GetUnitCell();
						RecalculateUnitPathMap(_currentUnit);
                        std::vector<PFWaveCell*> path = _currentUnit->_owner_w->_pathfinder->FindPathOnMap(p.x, p.y);
                    
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
                newCurrentUnit = _match->_currentPlayer_w->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit, false);
                _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
            }
        }
    }
    else if (_currentUnit && !_currentUnit->CanMove())
    {
        if (_currentUnit == newCurrentUnit || (tapToSameUnit && _needToOpenMenuOnNextTapToSameUnit))
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
            SelectNewUnit(newCurrentUnit);
        
        
        if (!newCurrentUnit)
            DeselectCurrentUnit(_removeFromLock);
        
    }
    //if (_currentUnit) {
    //    engine->SetPathZoneLevel(OBJECT_LEVEL_OVERGROUND);
    //}
}

void MAXGameController::SelectNewUnit(GameUnit* unit)
{
    if (_currentUnit)
    {
        _currentUnit->UnitDidDeselect();
        HideUnitPath();
        HidePathMap();
        _iputController->AbortCurrentAction();
    }
    _currentUnit = unit;
    _needToOpenMenuOnNextTapToSameUnit = _currentUnit && _currentUnit->_owner_w->GetIsCurrentPlayer();
    _gameInterface->HideUnitMenu();
    _currentUnit->UnitDidSelect();
    engine->SelectUnit(_currentUnit->GetUnitObject());
    _gameInterface->OnCurrentUnitChanged(unit, false, _match->PlayerIsEnemyToPlayer(unit->_owner_w, _match->_currentPlayer_w));
    
    if (_currentUnit->_owner_w->GetIsCurrentPlayer())
    {
        if (_currentUnit->CanMove())
        {
            ShowUnitPath(_currentUnit);
            RecalculateUnitPathMap(_currentUnit);
            ShowPathMap();
        }
        else if (_currentUnit->_unitData->GetIsTaskFinished())
        {
            GameUnitData* data = _currentUnit->_unitData;
            _iputController->StartSelectConstructorExitCell(_currentUnit, data->GetTaskSecondUnit());
        }
    }
}

void MAXGameController::ProceedLongTap(float tapx, float tapy)
{
    if (_freezeCounter1>0)
        return;
    if (_currentUnit && !_currentUnit->_owner_w->GetIsCurrentPlayer()) 
        return;
    
    
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    if (_iputController->ShoulTakeTap(p)) {
        _iputController->ProceedTap(tapx, tapy);
        if (_iputController->shouldDeselectUnit) {
            DeselectCurrentUnit(true);
        }
        
        return;
    }

    

    GameUnit* newCurrentUnit = _match->_currentPlayer_w->_agregator->GetUnitInPosition(p.x, p.y, NULL, _currentUnit, true);
    bool selectNew = !_currentUnit;
    if (_currentUnit && newCurrentUnit)
        selectNew = !_match->PlayerIsEnemyToPlayer(newCurrentUnit->_owner_w, _currentUnit->_owner_w);
    
    if (!selectNew)
    {
        if (newCurrentUnit)
        {
            if (_currentUnit == newCurrentUnit)
            {
                if (_needToOpenMenuOnNextTapToSameUnit)
                {
                    _gameInterface->ShowMenuForCurrentUni(this);
                    _needToOpenMenuOnNextTapToSameUnit = false;
                }
            }
            else
            {
                GameUnit *attackedUnit = _match->UnitForAttackingByUnit(_currentUnit, p);
                if (attackedUnit && attackedUnit != _currentUnit) 
				{
					vector<GameUnit*> units;
					units.push_back(_currentUnit);
					StartMultipleAttackSequence(units, attackedUnit, p, true);
                    _match->GameUnitDidInitiateFire(_currentUnit);
				}
            }
        }
        else
        {
            DeselectCurrentUnit(true);
        }
    }
    else
    {
        _iputController->AbortCurrentAction();
        if (newCurrentUnit)
            SelectNewUnit(newCurrentUnit);
    }
}

void MAXGameController::StartAttackSequence(GameUnit *agressor, GameUnit *target, const CCPoint &point)
{
    if (agressor == target)
        return;
    
    
	if (!_singleFire)
	{
		if (target && agressor->_owner_w->GetIsCurrentPlayer())
			SOUND->PlaySystemSound(SOUND_TYPE_UNIT_FIRING);
		if (target && target->_owner_w->GetIsCurrentPlayer() && !agressor->_owner_w->GetIsCurrentPlayer())
			SOUND->PlaySystemSound(SOUND_TYPE_ENEMY_FIRING_ON_UNIT);
	}

    _currentTargetUnit = target;
    int reslevel = 1;
    int alevel = agressor->GetConfig()->_bLevel;
    int tlevel = target?target->GetConfig()->_bLevel:alevel;
    
    if (tlevel == alevel)
        reslevel = tlevel + 1;
    else if (tlevel > alevel)
        reslevel = tlevel + 1;
    else if (tlevel < alevel)
        reslevel = alevel - 1;
    
    _currentFiringCell = point;
    agressor->Fire(point, reslevel);
	if (agressor->GetConfig()->_isBombMine) 
        agressor->Destroy();
    
}

void MAXGameController::StartMultipleAttackSequence(vector<GameUnit*> agressors, GameUnit *target, const CCPoint &point, bool singleFire)
{
    //AGA POPALSA!!!
	if (target)
	    target->AbortCurrentPath();

	if (_currentFiringUnits.size() != 0)
	{
		_attackSequences.push_back(pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>>(pair<GameUnit*, CCPoint>(target, point), agressors));
		return;
	}
	_currentFiringUnits = agressors;
	_singleFire = singleFire;
	StartAttackSequence(_currentFiringUnits[0], target, point);
}

void MAXGameController::DeselectCurrentUnit(bool _removeFromLock)
{
    _iputController->AbortCurrentAction();
    if (_currentUnit)
    {
        _currentUnit->UnitDidDeselect();
        if (_currentUnit->GetPath().size() > 0 && !_removeFromLock)
        {
            _currentUnit->ClearPath();
        }
        else
        {
            _gameInterface->OnCurrentUnitChanged(NULL, _removeFromLock, false);
            engine->SelectUnit(NULL);
            _currentUnit = NULL;
            HidePathMap();
        }				
        HideUnitPath();
		HidePathMap();
    }
}

void MAXGameController::TryStartConstruction(string type)
{
    if (_currentUnit->_unitData->ContainsCurrentTask() || !_iputController->GetRunedSpecialAction())
        return;
    _currentUnit->ClearPath();
    MAXObjectConfig* newUnitConfig = MAXConfigManager::SharedMAXConfigManager()->GetUnitConfig(type);
    if (_currentUnit->_unitData->GetIsBuilding())
    {
        
    }
    else
    {
        if (newUnitConfig->_bSize == 1)
        {
            //TODO: force MAXGAME to select path
            _iputController->StartSelectSmallBuildingConstructionPathAction(_currentUnit, newUnitConfig);
        }
        else
        {
            //force MAXGAME to select suitable place
            SOUND->PlaySystemSound(SOUND_TYPE_MOVE_TAPE_TO_BULD);
            _iputController->StartSelectLargeBuildingConstructionPlaceAction(_currentUnit, newUnitConfig);
        }
    }
    
    UpdateCurrentUnitPath();
    _gameInterface->HideUnitMenu();
}

#pragma mark - SelectedGameObjectDelegate

void MAXGameController::onUnitMoveStart(GameUnit* unit)
{
    IncreaseFreezeCounter();
}

void MAXGameController::onUnitMovePause(GameUnit* unit)
{
    DecreaseFreezeCounter();
    if (unit == _currentUnit)
    {
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
    }
}

void MAXGameController::onUnitMoveStepBegin(GameUnit* unit)
{
    if (unit == _currentUnit)
    {
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
    }
}

void MAXGameController::onUnitMoveStepEnd(GameUnit* unit)
{
    // check if unit path changed
    if (!CheckIfNextCellOk(unit))
    {
        unit->AbortCurrentPath();
        RecalculateUnitPath(unit);
    }
}

void MAXGameController::onUnitMoveStop(GameUnit* unit)
{
    DecreaseFreezeCounter();
    if (unit == _currentUnit)
    {
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
    }

	// check landing pad
	if (unit->_unitData->GetConfig()->_isPlane)
	{
		CCPoint location = unit->GetUnitCell();
		USimpleContainer<GameUnit*> *units = unit->_owner_w->_agregator->UnitsInCell(location.x, location.y);
		for (int i = 0; i < units->GetCount(); i++)
		{
			GameUnit* u = units->objectAtIndex(i);
			if (u->_unitData->GetConfig()->_isLanding && u->_owner_w == unit->_owner_w)
			{
				unit->Landing();
				break;
			}
		}
	}
}

void MAXGameController::onUnitFireStart(GameUnit* unit)
{
    IncreaseFreezeCounter();
    if (unit == _currentUnit)
    {
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
    }
}

void MAXGameController::onUnitFireStop(GameUnit* unit)
{
    MakePain();

    DecreaseFreezeCounter();
}

void MAXGameController::onUnidHided(GameUnit* unit)
{
	if (_gameInterface)
		_gameInterface->RemoveUnitFromLock(unit);
    if (_currentUnit == unit)
    {
        engine->SelectUnit(NULL);
		if (_gameInterface)
	        _gameInterface->OnCurrentUnitChanged(NULL, true, false);
        _currentUnit = NULL;
		HideUnitPath();
		HidePathMap();
    }
}

void MAXGameController::onUnitDestroyed(GameUnit* unit)
{
	bool search = true;
    onUnidHided(unit);
    
	while (search)
	{
		search = false;
		vector<pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>>>::iterator it = _attackSequences.begin();
		for (int i = 0; i < _attackSequences.size(); i++)
		{
			pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>> newSequence = _attackSequences[i];
			if (newSequence.first.first == unit)
			{
				search = true;
				_attackSequences.erase(it);
				break;
			}
			vector<GameUnit*> units = newSequence.second;
			for (int j = 0; j < units.size(); j++)
			{
				if (units[j] == unit)
				{
					search = true;
					_attackSequences.erase(it);
					break;
				}
			}
			if (search)
				break;
			it++;
		}
	}
}

bool MAXGameController::IsUnitSelected(GameUnit* unit)
{
    return unit == _currentUnit;
}
//void MAXGameController::onUnitFired(GameUnit* unit)
//{
//    if (unit->CanFire(_currentFiringCell)) 
//        return;
//    
//	bool search = true;
//	while (search)
//	{
//		search = false;
//		vector<pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>>>::iterator it = _attackSequences.begin();
//		for (int i = 0; i < _attackSequences.size(); i++)
//		{
//            //trget-agressors
//			pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>> newSequence = _attackSequences[i];
//			vector<GameUnit*> units = newSequence.second;
//			for (int j = 0; j < units.size(); j++)
//			{
//				if (units[j] == unit)
//				{
//					search = true;
//					_attackSequences.erase(it);
//					break;
//				}
//			}
//			if (search)
//				break;
//			it++;
//		}
//	}
//}

void MAXGameController::MakePain()
{
	GameUnit *_currentFiringUnit = _currentFiringUnits[0];
    if (!_currentTargetUnit) 
        _currentTargetUnit = _match->UnitForAttackingByUnit(_currentFiringUnit, _currentFiringCell);
    
	bool stillAlive = false;
    if (_currentTargetUnit && _currentFiringUnit)
    {
        // Do here damage calculation?
        // yes please
        // you are wellcome!!!!
		stillAlive = _currentTargetUnit->ReceiveDamage(_currentFiringUnit, 1);
		if (!stillAlive)
        {
			DestroyUnit(_currentTargetUnit);
			if (!_singleFire && _currentTargetUnit->_owner_w->GetIsCurrentPlayer())
				SOUND->PlaySystemSound(SOUND_TYPE_UNIT_DESTROYED);
        }
        else
        {
            if (_currentTargetUnit == _currentUnit)
                _gameInterface->OnCurrentUnitDataChanged(_currentTargetUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
        }
    }
	if (_currentFiringUnit->_unitData->GetShotBalance() == 0 || _currentFiringUnit->GetConfig()->_isBombMine)
		_currentFiringUnits.erase(_currentFiringUnits.begin());
    
	if (!stillAlive || _singleFire)
		_currentFiringUnits.clear();

	if (_currentFiringUnits.size() == 0 && _attackSequences.size() == 0)
	{
		_currentTargetUnit = NULL;
        if (_currentUnit)
        {
            if (_startAttackModeAgain && _currentUnit->_unitData->GetParameterValue(UNIT_PARAMETER_TYPE_SHOTS) >0)
                EnableModeForCurrentUnit(UNIT_MENU_ACTION_ATTACK);
            else
                ShowPathMap();
        }
		_startAttackModeAgain = false;
	}
	else
	{
		IncreaseFreezeCounter();
		MAXAnimationWait* delay = new MAXAnimationWait(0.75);
		delay->_delegate = this;
		_fireDelayAnim = delay;
		MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(delay);
	}
}

void MAXGameController::DestroyUnit(GameUnit* unit)
{
	unit->Destroy();
    bool isCurrent = unit == _currentUnit;
	this->onUnidHided(unit);
			
	if (isCurrent)
	{
		HideUnitPath();
		HidePathMap();
		_currentUnit = NULL;
	}
	else if (_currentUnit)
    {
		RecalculateUnitPathMap(_currentUnit);
        ShowPathMap();
	}
}

void MAXGameController::EnableModeForCurrentUnit(UNIT_MENU_ACTION action)
{
    _currentUnit->SetPath(vector<PFWaveCell*>());
    float range = 1.8;
    if (action == UNIT_MENU_ACTION_ATTACK)
        range = _currentUnit->_unitData->GetMaxParameterValue(UNIT_PARAMETER_TYPE_RANGE);
    
    _iputController->StartSelectSecondUnit(_currentUnit, range, action);
    HideUnitPath();
    if (!_iputController->UnitCanMoveWithAction())
        HidePathMap();
}

#pragma mark - GIUnitActionMenuDelegate

void MAXGameController::OnUnitMenuItemSelected(UNIT_MENU_ACTION action)
{
	if (!_currentUnit)
		return;
	
	
    _needToOpenMenuOnNextTapToSameUnit = true;
    _gameInterface->HideUnitMenu();
    if (action == UNIT_MENU_ACTION_DONE)
	{
        RecalculateUnitPath(_currentUnit);
		_currentUnit->ConfirmCurrentPath();
		HideUnitPath();
        return;
	}
    else if (action == UNIT_MENU_ACTION_PLACEMINES)
    {
        if (_currentUnit->_unitData->GetIsPlacingMines())
            _currentUnit->SetPlacingMines(false);
        else
            _currentUnit->SetPlacingMines(true);
    }
    else if (action == UNIT_MENU_ACTION_REMOVEMINES)
    {
        if (_currentUnit->_unitData->GetIsRemovingMines())
            _currentUnit->SetRemovingMines(false);
        else
            _currentUnit->SetRemovingMines(true);
    }
	else if (action == UNIT_MENU_ACTION_STOP)
	{
        if (_currentUnit->GetPath().size() > 0) {
            _currentUnit->ClearPath();
            HideUnitPath();
        }
        else if (_currentUnit->_unitData->ContainsCurrentTask())
        {
            _currentUnit->AbortConstructingUnit();
            RecalculateUnitPathMap(_currentUnit);
            ShowPathMap();
        }
        if (_currentUnit->_unitData->GetIsBuilding())
        {
            if (!_currentUnit->GetConfig()->_isAllwaysOn)
                if (_currentUnit->_unitData->_isOn)
                    _currentUnit->TurnOf();
        }
        else
		return;
	}
	else if (action == UNIT_MENU_ACTION_START)
	{
        if (_currentUnit->_unitData->GetIsBuilding())
        {
            if (!_currentUnit->GetConfig()->_isAllwaysOn)
                if (!_currentUnit->_unitData->_isOn)
                    _currentUnit->TurnOn();
        }
		//_currentUnit->StartBuildProcess();
		return;
	}
    bool _interfaceAction = false;
    switch (action) {
        case UNIT_MENU_ACTION_BUILD:
        {
            if (!_currentUnit->_unitData->GetIsBuilding())
            {
                vector<string> ableToConstruct = MAXConfigManager::SharedMAXConfigManager()->ConstructableUnitsForConstructorType(_currentUnit->GetConfig()->_bSelfCreatorType);
                TryStartConstruction(ableToConstruct[0]);
                _interfaceAction = true;
            }
            
        }break;
            
            
        case UNIT_MENU_ACTION_REMOVE:
        {
            _currentUnit->Destroy();
        }break;
            
		case UNIT_MENU_ACTION_INFO:
        case UNIT_MENU_ACTION_ALLOCATE:
        case UNIT_MENU_ACTION_BUYUPGRADES:
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
//        case UNIT_MENU_ACTION_ENTER:
//        case UNIT_MENU_ACTION_LOAD:
//        case UNIT_MENU_ACTION_XFER:
        case UNIT_MENU_ACTION_REPAIR:
        case UNIT_MENU_ACTION_RELOAD:
        case UNIT_MENU_ACTION_STEAL:
		    _needTargetUnit = true;
            break;
            
        default:
            break;
    }
    
    if (_needTargetUnit) {
        //select suitable target unit and perform action
        EnableModeForCurrentUnit(action);
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
    if (_currentUnit)
        _gameInterface->OnCurrentUnitDataChanged(_currentUnit, _match->PlayerIsEnemyToPlayer(_currentUnit->_owner_w, _match->_currentPlayer_w));
}

bool MAXGameController::IsUnitActionSelected(UNIT_MENU_ACTION action)
{
    if (action == UNIT_MENU_ACTION_PLACEMINES)
        return _currentUnit->_unitData->GetIsPlacingMines();
    if (action == UNIT_MENU_ACTION_REMOVEMINES)
        return _currentUnit->_unitData->GetIsRemovingMines();
    
    return false;
}

#pragma mark - GIUnitSelectionMenuDelegate

void MAXGameController::OnUnitSelected(GameUnit* result, const CCPoint &point)
{
    _gameInterface->HideUnitSelectionMenu();
	vector<GameUnit*> units;
	units.push_back(_currentUnit);
	StartMultipleAttackSequence(units, result, point, true);
    _match->GameUnitDidInitiateFire(_currentUnit);
}

#pragma mark - MAXAnimationDelegate

void MAXGameController::OnAnimationStart(MAXAnimationBase* animation)
{}

void MAXGameController::OnAnimationUpdate(MAXAnimationBase* animation)
{}

void MAXGameController::OnAnimationFinish(MAXAnimationBase* animation)
{
	if (animation == _endDelayAnim)
	{
		game->StopMatch();
	}
	if (animation == _fireDelayAnim)
	{
		_fireDelayAnim = NULL;
		if (_currentFiringUnits.size() == 0)
		{
			if (_attackSequences.size() != 0)
			{
				pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>> newSequence = _attackSequences[0];
				_attackSequences.erase(_attackSequences.begin());
				_currentTargetUnit = newSequence.first.first;
				_currentFiringCell = newSequence.first.second;
				_currentFiringUnits = newSequence.second;
			}
			else
			{
				DecreaseFreezeCounter();
				return;
			}
		}
        bool search = true;
        while (search)
        {
            search = false;
            vector<GameUnit*>::iterator it = _currentFiringUnits.begin();
            for (int i = 0; i < _currentFiringUnits.size(); i++)
            {
                if (!_currentFiringUnits[i]->CanFire(_currentFiringCell)) {
                    _currentFiringUnits.erase(it);
                    search = true;
                    break;
                }
                it ++;
            }
        }
        if (_currentFiringUnits.size() != 0)
            StartAttackSequence(_currentFiringUnits[0], _currentTargetUnit, _currentFiringCell);
        else
            _currentTargetUnit = NULL;
        
            
		DecreaseFreezeCounter();
	}
}

#pragma mark - GIMessageWindowDelegate 

void MAXGameController::OnMessageWindowButtonClicked(int buttonNumber, GIMessageWindow *sender)
{
    if (sender == _windowCloseQuestion) {
        _windowCloseQuestion = NULL;
        if (buttonNumber == 0)
        {
            EndMatch();
            _gameInterface->_gameFinished = true;
            _gameInterface->DisappearWindow(sender);
        }
        else
            _gameInterface->DisappearWindow(sender);
    }
}
