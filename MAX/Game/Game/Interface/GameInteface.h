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
class GIUnitActionMenu;
class GIUnitActionMenuDelegate;
class GIUnitSelectionMenu;
class GIUnitSelectionMenuDelegate;

class MAXGameController;

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
    
   
    GIUnitActionMenu* _unitMenu;
    GIUnitSelectionMenu* _unitSelectionMenu;
    
 
public:

	MAXGameController *_gameController;
    
    bool GetUnitMenuOpened() const {return _unitMenu != NULL;};
    bool GetSelectUnitMenuOpened() const {return _unitSelectionMenu != NULL;};
    
    void ShowMenuForCurrentUni(GIUnitActionMenuDelegate *delegate);
    void HideUnitMenu();
    void UpdateUnitMenuPosition();
    
    void ShowUnitSelectionMenu(GIUnitSelectionMenuDelegate *delegate, const vector<GameUnit*> units, const CCPoint &cellPoint);
    void HideUnitSelectionMenu();
    void UpdateUnitSelectionMenuPosition(const CCPoint &cellPoint);
    
    void ShowAgreedTarget(GameUnit *agressor, GameUnit *target, const CCPoint &cellPoint);
    void HideAgreedTarget();
    void UpdateAgreedTargetPosition(const CCPoint &cellPoint);
    
    void SetNodeNearCell(CCNode *node, const CCPoint &cellPoint, int size);
    
    bool GetDrawGrid() const {return _drawGrid;}
    bool ShouldReceiveTouch(int x, int y);
    
    void ClearLockedUnits();
    void RemoveUnitFromLock(GameUnit* unit);
    void SetLockedUnits(vector<GameUnit*> units);
    vector<GameUnit*> GetLockedUnits() const {return _lockedUnits;};
    void SetLockUnits(bool flag) {_lockUnits = flag; UpdateToggleLockUnitsButton();}
    
    CCPoint GetCenter();
    void SetCurrentTurnNumber(int number);
    void SetCurrentPlayerColor(Color color);
    
    GameInterface();
    ~GameInterface();
    
    void InitBaseInterface();
    
    void ShowUnitSpottedMessage(GameUnit* unit);

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
    
    void OnMenuItemButton(CCMenuItem* sender);
    
#pragma mark - Game events
    
    void OnCurrentUnitChanged(GameUnit* unit, bool removeFromLock);
    void OnCurrentUnitDataChanged(GameUnit* unit);

};

#endif /* defined(__MAX__GameInteface__) */
