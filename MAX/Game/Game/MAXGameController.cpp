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

#include "GameMatch.h"
#include "GameEffect.h"
#include "GameUnit.h"

MAXGameController::MAXGameController()
:_selectedUnit_w(NULL), _actionType(-1), _buildingConfig_w(NULL)
{

}

MAXGameController::~MAXGameController()
{
    
}

bool MAXGameController::StartSelectLargeBuildingConstructionPlaceAction(GameUnit* constructor,  MAXObjectConfig *buildingConfig)
{
    CCPoint newCell = constructor->GetUnitCell();
    do {
        bool canConstrutHere = game->_match->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w);
        if (canConstrutHere)
            break;
        
        newCell.x -= 1;
        canConstrutHere = game->_match->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w);
        if (canConstrutHere)
            break;
        
        newCell.x += 1;
        newCell.y -= 1;
        canConstrutHere = game->_match->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w);
        if (canConstrutHere)
            break;
        
        
        newCell.x -= 1;
        canConstrutHere = game->_match->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w);
        if (canConstrutHere)
            break;
        else
            return false;
        
    } while (false);
    
    _actionType = MAXGameControllerAction_SelectLargeBuildingConstructionPlace;
    
    _buildingConfig_w = buildingConfig;
    _selectedUnit_w = constructor;
    constructor->CreateLargeBuildingTape();
    _largeBuildingConstructionPlace = _selectedUnit_w->GetUnitCell();
    _secondaryObject_w = constructor->GetEffectUnder();
    _secondaryObject_w->SetLocation(newCell);
    return true;
}

void MAXGameController::ProceedPan(int speedx, int speedy)
{
    if (speedx == 0 && speedy == 0) 
        return;
    
    printf("%d, %d\n", speedx, speedy);
    switch (_actionType)
    {
        case MAXGameControllerAction_SelectLargeBuildingConstructionPlace:
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
            
            
            bool canConstrutHere = game->_match->GetCanConstructLargeBuildingInCell(newCell, _buildingConfig_w);
            if (canConstrutHere)
                _secondaryObject_w->SetLocation(newCell);
            
        }   break;
            
        default:
            break;
    }
}

void MAXGameController::ProceedTap(float tapx, float tapy)
{
    CCPoint p = engine->ScreenToWorldCell(CCPoint(tapx, tapy));
    p.x = floorf(p.x);
    p.y = floorf(p.y);
    
    switch (_actionType) {
        case MAXGameControllerAction_SelectLargeBuildingConstructionPlace:
        {
            CCPoint tapeLocation = _secondaryObject_w->GetUnitCell();
            if (p.x == tapeLocation.x || p.x == tapeLocation.x + 1) {
                if (p.y == tapeLocation.y || p.y == tapeLocation.y + 1) {
                    if (_delegate_w) 
                        _delegate_w->SelectLargeBuildingConstructionPlaceActionFinished(tapeLocation, _buildingConfig_w);
                    
                    
                    _actionType = -1;
                    _buildingConfig_w = NULL;
                    _selectedUnit_w = NULL;
                    
                    return;
                }
            }
            
            _selectedUnit_w->DestroyBuildingTape();
            _actionType = -1;
            _buildingConfig_w = NULL;
            _selectedUnit_w = NULL;
            
        }   break;
            
        default:
            break;
    }
}

