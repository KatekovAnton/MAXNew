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
#include "USimpleContainer.h"
#include "MAXEngine.h"
#include "GameObjectDelegate.h"
#include "GIUnitActionMenuDelegate.h"
#include "GIUnitSelectionMenuDelegate.h"
#include "MAXAnimationDelegate.h"
#include "MAXGameInputControllerDelegate.h"

class MAXGameInputController;
class GameMap;
class GameUnit;

class GameMatch;
class GameMatchPlayer;
class GameEffect;
class GameInterface;
class GamePathVisualizer;
class PFWaveCell;

class MAXGameController : public MAXEngineDelegate, public GameObjectDelegate, public GIUnitActionMenuDelegate, public MAXAnimationDelegate, public MAXGameInputControllerDelegate, public GIUnitSelectionMenuDelegate
{
    MAXAnimationBase* _fireDelayAnim;
	MAXAnimationBase* _endDelayAnim;
public:



	MAXGameInputController *_iputController;


	vector<pair<pair<GameUnit*, CCPoint>, vector<GameUnit*>>> _attackSequences;
	
    vector<GameUnit*> _currentFiringUnits;
    GameUnit *_currentTargetUnit;
    CCPoint  _currentFiringCell;
	bool _singleFire;
    void StartAttackSequence(GameUnit *agressor, GameUnit *target, const CCPoint &point);
    
    GameInterface *_gameInterface;
    GamePathVisualizer *_pathVisualizer;
 
    void ShowPathMap();
    void HidePathMap();
    void RefreshCurrentUnitPath();
    void RecalculateUnitPathMap(GameUnit *unit);
    void RecalculateUnitPath(GameUnit* unit);
    
    bool _needToOpenMenuOnNextTapToSameUnit;
    
    
    int _freezeCounter1;
    
public:
    
    void IncreaseFreezeCounter();
    void DecreaseFreezeCounter();
    
    GameMatch *_match;
    
    
    GameMatch *GetCurrentMatch() const {return _match;}
    GameUnit *_currentUnit;
    
    bool _startAttackModeAgain;
    
    
	MAXGameController();
    ~MAXGameController();

    
    void Init();
    void StartMatch();
	void EndMatch();
	void DeletionProgressDidChange(float zeroToOne);

    int CurrentPlayerId() const;
    
    void UpdateCurrentUnitPath();
    void TryStartConstruction(string type);
    void EnableModeForCurrentUnit(UNIT_MENU_ACTION action);
    void SelectNewUnit(GameUnit* unit);
    void DeselectCurrentUnit(bool _removeFromLock);
    bool EscapeStealthUnitFromPos(GameUnit* unit, const int x, const int y, GameMatchPlayer *reasonPlayer, vector<CCPoint> lockedCells);
	void StartMultipleAttackSequence(vector<GameUnit*> agressors, GameUnit *target, const CCPoint &point, bool singleFire);
    void MakePain();
	void DestroyUnit(GameUnit* unit);
	bool CheckIfNextCellOk(GameUnit* unit);
	void ShowUnitPath(GameUnit *unit);
	void HideUnitPath();
    

    bool EndTurn();
    

#pragma mark - Interaction	
    bool CanStartPinch(float x, float y);
    void ProceedPinch(float scale);
    void ProceedPan(float speedx, float speedy);
    void ProceedTap(float tapx, float tapy);
    void ProceedLongTap(float tapx, float tapy);
    
#pragma mark - Interface
#pragma mark Messages
    void ShowUnitSpottedMessage(GameUnit* unit);
    
#pragma mark - MAXGameInputControllerDelegate
    virtual void SelectLargeBuildingConstructionPlaceActionFinished(CCPoint result, MAXObjectConfig *buildingConfig);
    virtual void SelectSmallBuildingConstructionPathActionFinished(CCPoint result, MAXObjectConfig *buildingConfig);
    
    virtual void SelectSecondUnitActionCanceled();
    virtual void SelectSecondUnitActionFinished(const vector<GameUnit*> units, const CCPoint &cellPoint, UNIT_MENU_ACTION action);
    
	virtual GameMatch *GetCurrentMatch();

#pragma mark - MAXEngineDelegate
    virtual void onFrame();
    
#pragma mark - GameObjectDelegate
    virtual void onUnitMoveStart(GameUnit* unit);
	virtual void onUnitMovePause(GameUnit* unit);
	virtual void onUnitMoveStepBegin(GameUnit* unit);
	virtual void onUnitMoveStepEnd(GameUnit* unit);
    virtual void onUnitMoveStop(GameUnit* unit);
    virtual void onUnitFireStart(GameUnit* unit);
    virtual void onUnitFireStop(GameUnit* unit);

	virtual void onUnidHided(GameUnit* unit);
    virtual void onUnitDestroyed(GameUnit* unit);

#pragma mark - GIUnitActionMenuDelegate
    virtual void OnUnitMenuItemSelected(UNIT_MENU_ACTION action);
    
#pragma mark - GIUnitSelectionMenuDelegate 
    virtual void OnUnitSelected(GameUnit* result, const CCPoint &point);
    
#pragma mark - MAXAnimationDelegate 
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);

};

#endif /* defined(__MAX__MAXGameController__) */
