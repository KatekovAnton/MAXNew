//
//  MAXGameInputController.h
//  MAX
//
//  Created by Anton Katekov on 20.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXGameInputController__
#define __MAX__MAXGameInputController__

#include <iostream>
#include "miniPrefix.h"

class GameObject;
class GameEffect;
class GameUnit;

class MAXObjectConfig;
class MAXGameInputControllerDelegate;

enum MAXGameInputControllerAction
{
    MAXGameInputControllerAction_SelectLargeBuildingConstructionPlace,
    MAXGameInputControllerAction_SelectSmallBuildingConstructionPath,
    MAXGameInputControllerAction_SelectConstructorExitCell,
    MAXGameInputControllerAction_SelectSecondUnit,
    MAXGameInputControllerAction_SelectSpawnLocation
};

class MAXGameInputController {
    
public:
    
    bool shouldDeselectUnit;
    
    MAXGameInputControllerDelegate* _delegate_w;
    
    GameUnit *_selectedUnit_w;
    GameObject *_secondaryObject_w;
    CCPoint _largeBuildingConstructionPlace;
    MAXObjectConfig* _buildingConfig_w;
    float _distance;
    int _step;
    CCPoint _previousStepResult;
    UNIT_MENU_ACTION _action;
    vector<GameEffect*> _additionalEffects;
    vector<CCPoint> suitableCells;
    
    int _actionType;
    
    bool GetRunedSpecialAction() const {return _actionType == -1;};
    
    bool StartSelectLargeBuildingConstructionPlaceAction(GameUnit* constructor, MAXObjectConfig *buildingConfig);
    bool StartSelectSmallBuildingConstructionPathAction(GameUnit* constructor,  MAXObjectConfig *buildingConfig);
    bool StartSelectConstructorExitCell(GameUnit* constructor, GameUnit* createdUnit);
    
    bool StartSelectSecondUnit(GameUnit* selectedUnit, float maxDistance, UNIT_MENU_ACTION action);
    
	bool StartSelectPlayerSpawnLocation();
    

    void AbortCurrentAction();
    
    void ProceedPan(int speedx, int speedy);
    void ProceedTap(float tapx, float tapy);
    void ProceedPinch(float delta);
    
    bool ShoulTakePan() const { return _actionType != -1 && _actionType != MAXGameInputControllerAction_SelectConstructorExitCell&& _actionType != MAXGameInputControllerAction_SelectSecondUnit;  };
    bool ShoulTakeTap(const CCPoint &cell);
    bool ShoulTakePinch(float delta);
    bool UnitCanMoveWithAction();
    
    void OnGameStartsActons();
    void OnGameStopsActons();
    
    MAXGameInputController();
    ~MAXGameInputController();
    
};

#endif /* defined(__MAX__MAXGameInputController__) */
