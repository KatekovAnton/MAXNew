//
//  GameInteface.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameInteface__
#define __MAX__GameInteface__

#include <iostream>
#include "cocos2d.h"
#include "MAXEngine.h"

using namespace cocos2d;

class GameUnit;
class GIUnitParametersNode;

class GameInterface : public CCScene
{
    CCNode* _unitInterfaceBattle;
    
    GIUnitParametersNode* _unitParameters;
    
    CCMenuItemSprite* _toggleLockUnitsButton;
    CCMenuItemSprite* _toggleGridButton;
    CCMenuItemSprite* _toggleScanButton;
    CCMenuItemSprite* _toggleRangeButton;
    CCMenuItemSprite* _toggleShotsButton;
    CCMenuItemSprite* _toggleStatusButton;
    CCMenuItemSprite* _toggleResourcesButton;
    CCMenuItemSprite* _toggleFogButton;
    CCMenuItemSprite* _togglePathZone;
    CCNode* _panel;
    CCMenuItemSprite* _buttonTogglePanel;
    CCMenuItemSprite* _buttonEndTurn;
    
    //locking units
    vector<GameUnit*> _lockedUnits;
    GameUnit* _currentUnit;
    void RemoveUnitFromLock(GameUnit* unit);
    bool _lockUnits;
    
    bool _drawGrid;
    bool _drawScan;
    bool _drawRange;
    bool _drawShots;
    bool _drawStatus;
    bool _drawResources;
    bool _drawFog;
    bool _drawPathZone;
 
    void UpdateToggleLockUnitsButton();
    void UpdateToggleGridButton();
    void UpdateToggleScanButton();
    void UpdateToggleRangeButton();
    void UpdateToggleShotsButton();
    void UpdateToggleStatusButton();
    void UpdateToggleResourcesButton();
    void UpdateToggleFogButton();
    void UpdateTogglePathZone();
    
public:
    
    bool GetDrawGrid() const {return _drawGrid;}
    bool ShouldReceiveTouch(int x, int y);
    
    
    GameInterface();
    ~GameInterface();
    
    void InitBaseInterface();

#pragma mark - Button events
    
    void OnToggleLockUnits(CCMenuItem* sender);
    void OnToggleGrid(CCMenuItem* sender);
    void OnToggleScan(CCMenuItem* sender);
    void OnToggleRange(CCMenuItem* sender);
    void OnToggleShots(CCMenuItem* sender);
    void OnToggleStatus(CCMenuItem* sender);
    void OnToggleResources(CCMenuItem* sender);
    void OnToggleFog(CCMenuItem* sender);
    void OnTogglePathZone(CCMenuItem* sender);

    void OnTogglePanel(CCMenuItem* sender);
    
    void OnEndTurn(CCMenuItem* sender);
    
#pragma mark - Game events
    
    void OnCurrentUnitChanged(GameUnit* unit);

};

#endif /* defined(__MAX__GameInteface__) */
