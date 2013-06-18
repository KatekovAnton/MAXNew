//
//  MAXGameController.h
//  MAX
//
//  Created by Anton Katekov on 20.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXGameController__
#define __MAX__MAXGameController__

#include <iostream>
#include "miniPrefix.h"

class GameObject;
class GameEffect;
class GameUnit;

class MAXObjectConfig;
class MAXGameControllerDelegate;

enum MAXGameControllerAction
{
    MAXGameControllerAction_SelectLargeBuildingConstructionPlace,
    MAXGameControllerAction_SelectSmallBuildingConstructionPath,
    MAXGameControllerAction_SelectConstructorExitCell,
    MAXGameControllerAction_SelectSecondUnit
};

class MAXGameController {
    
public:
    
    bool shouldDeselectUnit;
    
    MAXGameControllerDelegate* _delegate_w;
    
    GameUnit *_selectedUnit_w;
    GameObject *_secondaryObject_w;
    CCPoint _largeBuildingConstructionPlace;
    MAXObjectConfig* _buildingConfig_w;
    float _distance;
    UNIT_MENU_ACTION _action;
    vector<GameEffect*> _additionalEffects;
    vector<CCPoint> suitableCells;
    
    int _actionType;
    
    bool GetRunedSpecialAction() const {return _actionType == -1;};
    
    bool StartSelectLargeBuildingConstructionPlaceAction(GameUnit* constructor, MAXObjectConfig *buildingConfig);
    bool StartSelectSmallBuildingConstructionPathAction(GameUnit* constructor,  MAXObjectConfig *buildingConfig);
    bool StartSelectConstructorExitCell(GameUnit* constructor, GameUnit* createdUnit);
    
    
    bool StartSelectSecondUnit(GameUnit* selectedUnit, float maxDistance, UNIT_MENU_ACTION action);
    
    void AbortCurrentAction();
    
    void ProceedPan(int speedx, int speedy);
    void ProceedTap(float tapx, float tapy);
    
    bool ShoulTakePan() const { return _actionType != -1 && _actionType != MAXGameControllerAction_SelectConstructorExitCell&& _actionType != MAXGameControllerAction_SelectSecondUnit;  };
    bool ShoulTakeTap(const CCPoint &cell);
    
    MAXGameController();
    ~MAXGameController();
    
};

#endif /* defined(__MAX__MAXGameController__) */
