//
//  MAXGameInputController.cpp
//  MAX
//
//  Created by Anton Katekov on 20.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXGameController.h"
#include "MAXGameInputController.h"

#include "MAXEngine.h"
#include "GameMatch.h"
#include "GameEffect.h"
#include "GameUnit.h"
#include "GameUnitData.h"
#include "MAXAnimationPrefix.h"

#include "MAXUnitObject.h"
#include "MatchMapAgregator.h"
#include "PFWaveCell.h"
#include "Pathfinder.h"
#include "GameMap.h"


CCPoint findInterimPointWithLargeBuilding(CCPoint buildingLocation, CCPoint exitCell)
{
    CCPoint result = ccp(buildingLocation.x + 1, buildingLocation.y + 1);
    if (exitCell.x <= buildingLocation.x)
        result.x = buildingLocation.x;
    if (exitCell.y <= buildingLocation.y)
        result.y = buildingLocation.y;
    return result;
}

MAXGameInputController::MAXGameInputController()
:_selectedUnit_w(NULL), _actionType(-1), _buildingConfig_w(NULL)
{

}

MAXGameInputController::~MAXGameInputController()
{
    
}

bool MAXGameInputController::ShoulTakeTap(const CCPoint &cell)
{
    switch (_actionType) {
        case MAXGameInputControllerAction_SelectSecondUnit:
        {
            if (_action == UNIT_MENU_ACTION_ATTACK) 
                return true;
            
            if (_selectedUnit_w->_unitData->IsInRadius(cell, 0.9))
                return false;
            
            if (_action == UNIT_MENU_ACTION_RELOAD || _action == UNIT_MENU_ACTION_REPAIR || _action == UNIT_MENU_ACTION_DISABLE || _action == UNIT_MENU_ACTION_STEAL)
            {
				USimpleContainer<GameUnit*> *units = _delegate_w->GetCurrentMatch()->_currentPlayer_w->_agregator->UnitsInCell(cell.x, cell.y);
                for (int i = 0; i < units->GetCount(); i++)
                {
                    GameUnit* currentUnit = units->objectAtIndex(i);
                    if (_delegate_w->GetCurrentMatch()->UnitCanInteractWithUnit(_selectedUnit_w, currentUnit))
                        return true;
                }
                return false;
            }
            

        } break;
            
        default:
            break;
    }
    return _actionType != -1;
}

bool MAXGameInputController::ShoulTakePinch(float delta)
{
    return false;
}

bool MAXGameInputController::UnitCanMoveWithAction()
{
    return _actionType == -1 || (_actionType == MAXGameInputControllerAction_SelectSecondUnit && _action != UNIT_MENU_ACTION_ATTACK);
}

bool MAXGameInputController::StartSelectLargeBuildingConstructionPlaceAction(GameUnit* constructor,  MAXObjectConfig *buildingConfig)
{
    AbortCurrentAction();
    
    _buildingConfig_w = buildingConfig;
    CCPoint newCell = constructor->GetUnitCell();
    do {
        constructor->_currentlyProcesedConstructor = true;
        bool canConstrutHere = _delegate_w->GetCurrentMatch()->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w, constructor);
        if (canConstrutHere)
            break;
        
        newCell.x -= 1;
        canConstrutHere = _delegate_w->GetCurrentMatch()->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w, constructor);
        if (canConstrutHere)
            break;
        
        newCell.x += 1;
        newCell.y -= 1;
        canConstrutHere = _delegate_w->GetCurrentMatch()->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w, constructor);
        if (canConstrutHere)
            break;
        
        newCell.x -= 1;
        canConstrutHere = _delegate_w->GetCurrentMatch()->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w, constructor);
        constructor->_currentlyProcesedConstructor = false;
        if (canConstrutHere)
            break;
        else
            return false;
        
    } while (false);
    
    constructor->_currentlyProcesedConstructor = false;
    _actionType = MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace;
    
    _selectedUnit_w = constructor;
    constructor->CreateLargeBuildingTape();
    _largeBuildingConstructionPlace = _selectedUnit_w->GetUnitCell();
    _secondaryObject_w = constructor->GetEffectUnder();
    _secondaryObject_w->SetLocation(newCell);
    return true;
}

bool MAXGameInputController::StartSelectSmallBuildingConstructionPathAction(GameUnit* constructor,  MAXObjectConfig *buildingConfig)
{
    AbortCurrentAction();
    _actionType = MAXGameInputControllerAction_SelectSmallBuildingConstructionPath;
    CCPoint newCell = constructor->GetUnitCell();
    constructor->CreateSmallBuildingTape();
    
    _delegate_w->SelectSmallBuildingConstructionPathActionFinished(newCell, buildingConfig);
    AbortCurrentAction();
    return true;
}

bool MAXGameInputController::StartSelectConstructorExitCell(GameUnit* constructor, GameUnit* createdUnit)
{
    AbortCurrentAction();
    _actionType = MAXGameInputControllerAction_SelectConstructorExitCell;
    
    _selectedUnit_w = constructor;
    _secondaryObject_w = createdUnit;
    vector<CCPoint> cells = createdUnit->GetFullNearbyCells();
    GameMatch* match = _delegate_w->GetCurrentMatch();
    for (int i = 0; i < cells.size(); i++) {
        CCPoint cell = cells[i];
        if (match->UnitCanBePlacedToCell(cell.x, cell.y, (UNIT_MOVETYPE)constructor->GetConfig()->_bMoveType, constructor->_owner_w)) {
            suitableCells.push_back(cell);
            GameEffect* e = GameEffect::CreateExitPlaceMarker(constructor->GetConfig()->_bLevel);
            e->SetLocation(cell);
            e->Show();
            _additionalEffects.push_back(e);
        }
    }
    _selectedUnit_w->DestroyCheckIcon();
    return true;
}

bool MAXGameInputController::StartSelectSecondUnit(GameUnit* selectedUnit, float maxDistance, UNIT_MENU_ACTION action)
{
    AbortCurrentAction();
    _actionType = MAXGameInputControllerAction_SelectSecondUnit;
    _selectedUnit_w = selectedUnit;
    _distance = maxDistance;
    _action = action;
    _step = 0;
    if (action == UNIT_MENU_ACTION_ATTACK) {
        Color c = {255, 0, 0, 50};
        engine->SetOptionalZoneColor(c);
    }
    else
    {
        Color c = {255, 255, 255, 50};
        engine->SetOptionalZoneColor(c);
    }
    
    BoundingBox b = _selectedUnit_w->_unitData->GetBoundingBox(_selectedUnit_w->GetUnitCell(), maxDistance);
    engine->ClearOptionalZone();
    
    int level = selectedUnit->GetConfig()->_bLevel;
    bool canAttackOwnCell = false;
    bool notAllCells = false;
    if (action==UNIT_MENU_ACTION_ATTACK)
    {
        int fireType = selectedUnit->GetConfig()->_pFireType;
        int movetype = selectedUnit->GetConfig()->_bMoveType;
        if (fireType < 3 && movetype == UNIT_MOVETYPE_AIR)
            level = OBJECT_LEVEL_OVERUNITS;
        else if (fireType >= 3)
            level = OBJECT_LEVEL_OVERAIR;
        else if (movetype != UNIT_MOVETYPE_AIR)
            level = OBJECT_LEVEL_OVERUNITS;
        canAttackOwnCell = (movetype == UNIT_MOVETYPE_AIR && (fireType == 1 || fireType == 4)) || ((movetype != UNIT_MOVETYPE_AIR) && fireType == 3);
        notAllCells = _selectedUnit_w->GetConfig()->_pFireType == 2;
    }
    else
        level = OBJECT_LEVEL_OVERAIR;
    
    engine->SetOptionalZoneLevel((OBJECT_LEVEL)level);
    for (float i = b.min.y; i <= b.max.y; i += 1.0)
    {
        for (float j = b.min.x; j <= b.max.x; j += 1.0)
        {
            CCPoint p = ccp(j, i);
            bool needColor = _selectedUnit_w->_unitData->IsInRadius(p, maxDistance);
            if (!canAttackOwnCell)
                needColor &= !selectedUnit->_unitData->IsCellOfUnit(p);
            
            if (notAllCells) {
                GROUND_TYPE type = _delegate_w->GetCurrentMatch()->_map->GroundTypeAtXY(j, i);
                if (!(type == GROUND_TYPE_COAST || type == GROUND_TYPE_WATER))
                    needColor = false;
            }
            
            if (needColor) {
                engine->AddOptionalZoneCell(j, i);
            }
        }
    }
    return true;
}

bool MAXGameInputController::StartSelectPlayerSpawnLocation()
{
	AbortCurrentAction();
    _actionType = MAXGameInputControllerAction_SelectSpawnLocation;
    
	BoundingBox b;
	b.min.x = 0;
	b.max.y = 0;

	b.max.x = _delegate_w->GetCurrentMatch()->_map->GetMapWidth();
	b.max.y = _delegate_w->GetCurrentMatch()->_map->GetMapHeight();

	Color c = {255, 255, 255, 150};
	engine->SetOptionalZoneColor(c);
	engine->SetOptionalZoneLevel(OBJECT_LEVEL_OVERAIR);

	const int radius = 4;

    for (float i = b.min.y; i <= b.max.y; i += 1.0)
    {
        for (float j = b.min.x; j <= b.max.x; j += 1.0)
        {
            CCPoint p = ccp(j, i);
		}
	}

	return true;
}

void MAXGameInputController::AbortCurrentAction()
{
    switch (_actionType)
    {
        case MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace:
        {
            _selectedUnit_w->DestroyBuildingTape();
            _buildingConfig_w = NULL;
            _selectedUnit_w = NULL;
            _secondaryObject_w = NULL;
        } break;
        case MAXGameInputControllerAction_SelectConstructorExitCell:
        {
            if (_selectedUnit_w->_unitData->ContainsCurrentTask()) 
                _selectedUnit_w->CreateCheckIcon();
        } break;
        case MAXGameInputControllerAction_SelectSecondUnit:
        {
            engine->ClearOptionalZone();
        } break;
        default:
            break;
    }
    
    for (int i = 0; i < _additionalEffects.size(); i++)
    {
        GameEffect* e = _additionalEffects[i];
        e->Hide();
        delete e;
    }
    suitableCells.clear();
    _additionalEffects.clear();
    _actionType = -1;
}

void MAXGameInputController::ProceedPan(int speedx, int speedy)
{
    if (speedx == 0 && speedy == 0) 
        return;
    
    switch (_actionType)
    {
        case MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace:
        {
            CCPoint lastSelectedCell = _secondaryObject_w->GetUnitCell();
            
            CCPoint newCell = _selectedUnit_w->GetUnitCell();
            if (speedx<0)
                newCell.x = newCell.x - 1;
            if (speedy<0)
                newCell.y = newCell.y - 1;
            
            if (speedy == 0) 
                newCell.y = lastSelectedCell.y;
            if (speedx == 0)
                newCell.x = lastSelectedCell.x;
            
            
            _selectedUnit_w->_currentlyProcesedConstructor = true;
            bool canConstrutHere = _delegate_w->GetCurrentMatch()->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w, _selectedUnit_w);
            _selectedUnit_w->_currentlyProcesedConstructor = false;
            if (canConstrutHere)
                _secondaryObject_w->SetLocation(newCell);
            
        }   break;
            
        default:
            break;
    }
}

void MAXGameInputController::ProceedTap(float tapx, float tapy)
{
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    
    switch (_actionType) {
        case MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace:
        {
            CCPoint tapeLocation = _secondaryObject_w->GetUnitCell();
            if (p.x == tapeLocation.x || p.x == tapeLocation.x + 1) {
                if (p.y == tapeLocation.y || p.y == tapeLocation.y + 1) {
					shouldDeselectUnit = false;
                    if (_delegate_w) 
                        _delegate_w->SelectLargeBuildingConstructionPlaceActionFinished(tapeLocation, _buildingConfig_w);
                    
                    
                    _actionType = -1;
                    _buildingConfig_w = NULL;
                    _selectedUnit_w = NULL;
                    
                    return;
                }
            }
            
            AbortCurrentAction();
        } break;
        case MAXGameInputControllerAction_SelectConstructorExitCell:
        {
            shouldDeselectUnit = true;
            for (int i = 0; i < suitableCells.size(); i++)
            {
                CCPoint cell = suitableCells[i];
                if (cell.x == p.x && cell.y == p.y)
                {
                    MAXUnitObject* _unitObject = _selectedUnit_w->GetUnitObject();
                    CCPoint _unitCell = _unitObject->GetObjectCell();
                    
                    GameUnit* createdUnit = _selectedUnit_w->_unitData->GetTaskSecondUnit();
                    
                    
                    _selectedUnit_w->ClearPath();
                    
                    std::vector<PFWaveCell*> path;
                    Pathfinder* p = _selectedUnit_w->_owner_w->_pathfinder;
                    UNIT_MOVETYPE moveType = (UNIT_MOVETYPE)_selectedUnit_w->GetConfig()->_bMoveType;
                    p->MakePathMap(_unitCell.x, _unitCell.y, moveType, _selectedUnit_w->_unitData->GetMoveBalance());
                    createdUnit->_currentlyProcesedConstructor = true;
                    if (createdUnit->GetConfig()->_bSize == 1)
                    {
                        int direction = MAXObject::CalculateImageIndex(_unitCell, cell);
                        path.push_back(new PFWaveCell(cell.x, cell.y, p->GetMapCostAt(cell.x, cell.y, direction, moveType), direction));
                        
                        path.push_back(new PFWaveCell(_unitCell.x, _unitCell.y, 0, 0));
                    }
                    else
                    {
                        CCPoint wayPoit = findInterimPointWithLargeBuilding(createdUnit->GetUnitCell(), cell);
                        
                        int direction = MAXObject::CalculateImageIndex(wayPoit, cell);
                        path.push_back(new PFWaveCell(cell.x, cell.y, p->GetMapCostAt(cell.x, cell.y, direction, moveType), direction));
                        
                        if (_unitCell.x != wayPoit.x || _unitCell.y != wayPoit.y)
                        {
                            direction = MAXObject::CalculateImageIndex(_unitCell, wayPoit);
                            path.push_back(new PFWaveCell(wayPoit.x, wayPoit.y, 0, direction));
                        }
                        
                        path.push_back(new PFWaveCell(_unitCell.x, _unitCell.y, 0, 0));
                    }
                    createdUnit->_currentlyProcesedConstructor = false;
                    _selectedUnit_w->SetPath(path);
                    _selectedUnit_w->ConfirmCurrentPath();
                    _selectedUnit_w->_unitData->CompletlyFinishTask();
                    createdUnit->RemoveUnitFromMap();
                    createdUnit->EndConstructionSequense();
                    createdUnit->PlaceUnitOnMap();
                    _selectedUnit_w->DestroyCheckIcon();
                    _selectedUnit_w->DestroyBuildingTape();
                    shouldDeselectUnit = false;
                    break;
                }
            }
            
            AbortCurrentAction();
            
        } break;
        case MAXGameInputControllerAction_SelectSecondUnit:
        {
            shouldDeselectUnit = false;
            if (_selectedUnit_w->_unitData->IsInRadius(p, _distance))
            {
                if (_action == UNIT_MENU_ACTION_ATTACK)
                {
                    USimpleContainer<GameUnit*> *units = _delegate_w->GetCurrentMatch()->_currentPlayer_w->_agregator->UnitsInCell(p.x, p.y);
                    vector<GameUnit*> suitableUnits;
                    for (int i = 0; i < units->GetCount(); i++)
                    {
                        GameUnit* currentUnit = units->objectAtIndex(i);
                        if (_delegate_w->GetCurrentMatch()->UnitCanAttackUnit(_selectedUnit_w, currentUnit) && _selectedUnit_w != currentUnit)
                            suitableUnits.push_back(currentUnit);
                    }
                    AbortCurrentAction();
                    _delegate_w->SelectSecondUnitActionFinished(suitableUnits, p, _action);
                }
                else if (_action == UNIT_MENU_ACTION_RELOAD || _action == UNIT_MENU_ACTION_REPAIR || _action == UNIT_MENU_ACTION_STEAL || _action == UNIT_MENU_ACTION_DISABLE)
                {
                    bool wantSamePlayerUnut = ! (_action == UNIT_MENU_ACTION_STEAL || _action == UNIT_MENU_ACTION_DISABLE);
                    
					USimpleContainer<GameUnit*> *units = _delegate_w->GetCurrentMatch()->_currentPlayer_w->_agregator->UnitsInCell(p.x, p.y);
                    vector<GameUnit*> suitableUnits;
                    for (int i = 0; i < units->GetCount(); i++)
                    {
                        GameUnit* currentUnit = units->objectAtIndex(i);
                        if (_delegate_w->GetCurrentMatch()->UnitCanInteractWithUnit(_selectedUnit_w, currentUnit) && _selectedUnit_w != currentUnit && (_selectedUnit_w->_owner_w == currentUnit->_owner_w && wantSamePlayerUnut))
                            suitableUnits.push_back(currentUnit);
                    }
                    AbortCurrentAction();
                    _delegate_w->SelectSecondUnitActionFinished(suitableUnits, p, _action);
                }
            }
            else
            {
                AbortCurrentAction();
                _delegate_w->SelectSecondUnitActionCanceled();
            }
        } break;
            
        default:
            break;
    }
}

void MAXGameInputController::ProceedPinch(float delta)
{
}

void MAXGameInputController::OnGameStartsActons()
{
    switch (_actionType) {
        case MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace:
        {
            
        } break;
        case MAXGameInputControllerAction_SelectSmallBuildingConstructionPath:
        {
        
        } break;
        case MAXGameInputControllerAction_SelectConstructorExitCell:
        {
            
        } break;
        case MAXGameInputControllerAction_SelectSecondUnit:
        {
            engine->ClearOptionalZone();
        } break;
            
        default:
            break;
    }
}

void MAXGameInputController::OnGameStopsActons()
{
    switch (_actionType) {
        case MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace:
        {
            
        } break;
        case MAXGameInputControllerAction_SelectSmallBuildingConstructionPath:
        {
            
        } break;
        case MAXGameInputControllerAction_SelectConstructorExitCell:
        {
            
        } break;
        case MAXGameInputControllerAction_SelectSecondUnit:
        {
            StartSelectSecondUnit(_selectedUnit_w, _distance, _action);
        } break;
            
        default:
            break;
    }
}
