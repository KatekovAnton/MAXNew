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
class GameUnit;

class MAXObjectConfig;
class MAXGameControllerDelegate;

enum MAXGameControllerAction
{
    MAXGameControllerAction_SelectLargeBuildingConstructionPlace,
    MAXGameControllerAction_SelectSmallBuildingConstructionPath,
};

class MAXGameController {
    
public:
    
    MAXGameControllerDelegate* _delegate_w;
    
    GameUnit *_selectedUnit_w;
    GameObject *_secondaryObject_w;
    CCPoint _largeBuildingConstructionPlace;
    MAXObjectConfig* _buildingConfig_w;
    
    int _actionType;
    
    bool GetRunedSpecialAction() const {return _actionType == -1;};
    
    bool StartSelectLargeBuildingConstructionPlaceAction(GameUnit* constructor, MAXObjectConfig *buildingConfig);
    
    void ProceedPan(int speedx, int speedy);
    void ProceedTap(float tapx, float tapy);
    
    bool ShoulTakePan() const { return _actionType != -1; };
    bool ShoulTakeTap() const { return _actionType != -1; };
    
    MAXGameController();
    ~MAXGameController();
    
};

#endif /* defined(__MAX__MAXGameController__) */
