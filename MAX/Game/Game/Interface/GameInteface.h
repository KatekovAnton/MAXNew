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
#include "GIWindowsManagerDelegate.h"

using namespace cocos2d;

class GameUnit;
class GIWindow;
class GIUnitParametersNode;
class GIUnitActionMenu;
class GIUnitActionMenuDelegate;
class GIUnitSelectionMenu;
class GIUnitSelectionMenuDelegate;
class GIWindowsManager;

class MAXGameController;
class CCMenuItemNodes;
class NodeHieraclyOpacity;

class GameInterface : public CCScene, public GIWindowsManagerDelegate
{
    bool _inited;
    bool _firstTime;
    NodeHieraclyOpacity *_nodeHieraclyOpacity;
    
    CCNode* _unitInterfaceBattle;
    
    GIUnitParametersNode* _unitParameters;
    
	
    CCMenuItemNodes* _buttonOptions;

    CCMenuItemNodes* _toggleLockUnitsButton;
    CCMenuItemNodes* _toggleGridButton;
    CCMenuItemNodes* _toggleScanButton;
    CCMenuItemNodes* _toggleRangeButton;
    CCMenuItemNodes* _toggleShotsButton;
    CCMenuItemNodes* _toggleStatusButton;
    CCMenuItemNodes* _toggleResourcesButton;
    CCMenuItemNodes* _togglePathZone;
    CCLayerColor* _panel;
    CCMenuItemSprite* _buttonTogglePanel;
    CCMenuItemNodes* _buttonEndTurn;
    
    CCLabelTTF *_labelTurnNumber;
    CCLayerColor *_layerPlayerColor;
    
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
    void UpdateTogglePathZone();
    
   
    GIUnitActionMenu* _unitMenu;
    GIUnitSelectionMenu* _unitSelectionMenu;
    
    bool _visible;
    
    GIWindowsManager *_windowManager;
 
public:
    
    bool _gameFinished;

	MAXGameController *_gameController;
    
    
    void SetNewTurnData(int turnNumber, Color playerColor);
    
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

    void ToggleInterfaceVisibility(float visibleFlag);
    void PresentWindow(GIWindow* window);
    void DisappearWindow(GIWindow* window);

#pragma mark - GIWindowManagerDelegate
    
    virtual void WindowManagerDidCloseLastWindow();
    virtual bool WindowManagerShouldMoveToFinishState();
    
#pragma mark - Button events
    
    
	void OnOptions(CCObject* sender);
    void OnToggleLockUnits(CCObject* sender);
    void OnToggleGrid(CCObject* sender);
    void OnToggleScan(CCObject* sender);
    void OnToggleRange(CCObject* sender);
    void OnToggleShots(CCObject* sender);
    void OnToggleStatus(CCObject* sender);
    void OnToggleResources(CCObject* sender);
    void OnTogglePathZone(CCObject* sender);

    void OnTogglePanel(CCObject* sender);
    
    void OnEndTurn(CCObject* sender);
    
    void OnMenuItemButton(CCObject* sender);
    void OnToggleInterface(CCObject* sender);
    
#pragma mark - Game events
    
    void OnCurrentUnitChanged(GameUnit* unit, bool removeFromLock, bool isEnemyUnit);
    void OnCurrentUnitDataChanged(GameUnit* unit, bool isEnemyUnit);

};

#endif /* defined(__MAX__GameInteface__) */
