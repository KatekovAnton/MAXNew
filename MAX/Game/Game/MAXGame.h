//
//  MAXGame.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXGame__
#define __MAX__MAXGame__

#include <iostream>
#include "DisplayPinchDelegate.h"
#include "USimpleContainer.h"
#include "MAXEngine.h"
#include "SelectedGameObjectDelegate.h"
#include "GIUnitActionMenuDelegate.h"
#include "MAXAnimationDelegate.h"
#include "MAXGameControllerDelegate.h"

using namespace std;
using namespace Utils;

enum MAXGAMESTATE
{
    MAXGAMESTATE_LOADINGSTART,
    MAXGAMESTATE_MAINMENU,
    MAXGAMESTATE_LOADINGGAME,
    MAXGAMESTATE_GAME
};

class MAXEngine;
class GameMap;
class GameUnit;

class GameMatch;
class GameMatchPlayer;
class GameEffect;
class GameInterface;
class GamePathVisualizer;
class PFWaveCell;

class MAXGameController;

class MAXGame : public DisplayPinchDelegate, public MAXEngineDelegate, public SelectedGameObjectDelegate, public GIUnitActionMenuDelegate, public MAXAnimationDelegate, public MAXGameControllerDelegate
{
    
    MAXGAMESTATE _currentState;
    MAXAnimationBase* _waitTestAnimCorvette;
    MAXAnimationBase* _waitTestAnimCorvetteMovement;
    MAXAnimationBase* _waitTestAnimSubmarine;
    MAXAnimationBase* _waitTestAnimSubmarineMovement;
    GameUnit *_testUnitCorvette;
    GameUnit *_testUnitSubmarine;
    
    GameInterface *_gameInterface;
    GamePathVisualizer *_pathVisualizer;
    int iteration;
    void StartTest();
    void ShowPathMap();
    void HidePathMap();
    void RefreshCurrentUnitPath();
    void RecalculateUnitPathMap(GameUnit *unit);
    void RecalculateUnitPath(GameUnit* unit);
    
    bool _needToOpenMenuOnNextTapToSameUnit;
    
public:
    
    MAXGameController *_gameController;
    
    bool CheckIfNextCellOk(GameUnit* unit);
	void ShowUnitPath(GameUnit *unit);
	void HideUnitPath();
    
    int _freezeCounter;
    
    GameMatch *_match;
    
    USimpleContainer<GameEffect*> *_effects;
    
    MAXGAMESTATE GetCurrentState() const {return _currentState;}
    GameMatch *GetCurrentMatch() const {return _match;}
    GameUnit *_currentUnit;
    
    
    MAXGame();
    ~MAXGame();
    
    void Init();
    int CurrentPlayerId() const;
    
    void UpdateCurrentUnitPath();
    void TryStartConstruction(string type);
    void DeselectCurrentUnit(bool _removeFromLock);
    
    void StartMatch();
    bool EndTurn();
    void FlushEffectsWithNew(GameEffect *effect);
    
    bool EscapeStealthUnitFromPos(GameUnit* unit, const int x, const int y, GameMatchPlayer *reasonPlayer, vector<CCPoint> lockedCells);
  
    void UnidDidHide(GameUnit* unit);
    
#pragma mark - Interface
#pragma mark Messages
    void ShowUnitSpottedMessage(GameUnit* unit);
    
#pragma mark - MAXGameControllerDelegate
    virtual void SelectLargeBuildingConstructionPlaceActionFinished(CCPoint result, MAXObjectConfig *buildingConfig);
    virtual void SelectSmallBuildingConstructionPathActionFinished(CCPoint result, MAXObjectConfig *buildingConfig);
    
#pragma mark - MAXEngineDelegate
    virtual void onFrame();
    
#pragma mark - DisplayPinchDelegate
    virtual bool CanStartPinch(float x, float y);
    virtual void ProceedPinch(float scale);
    virtual void ProceedPan(float speedx, float speedy);
    virtual void ProceedTap(float tapx, float tapy);
    virtual void ProceedLongTap(float tapx, float tapy);
    
#pragma mark - SelectedGameObjectDelegate
    virtual void onUnitMoveStart(GameUnit* unit);
	virtual void onUnitMovePause(GameUnit* unit);
	virtual void onUnitMoveStepBegin(GameUnit* unit);
	virtual void onUnitMoveStepEnd(GameUnit* unit);
    virtual void onUnitMoveStop(GameUnit* unit);
    virtual void onUnitFireStart(GameUnit* unit);
    virtual void onUnitFireStop(GameUnit* unit);

#pragma mark - GIUnitActionMenuDelegate
    virtual void OnUnitMenuItemSelected(UNIT_MENU_ACTION action);
    
#pragma mark - MAXAnimationDelegate 
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
};

extern MAXGame *  game;

#endif /* defined(__MAX__MAXGame__) */
