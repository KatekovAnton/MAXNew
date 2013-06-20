//
//  GameUnitData.h
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameUnitData__
#define __MAX__GameUnitData__

#include <iostream>
#include "miniPrefix.h"

class MAXObjectConfig;
class GameUnitParameters;

class GUTask;
class GameUnit;

class GameUnitData {
    
    GUTask *_currentTask;
    bool _paused;
    
public:
    
    int _ownerId;
    
    //this building is currently constructing
    bool _isConstruction;
    bool _isOn;
    CCPoint _unitCell;
    
    GameUnitParameters* _unitParameters;
    
    bool _disabledByInfiltrator;
    bool _isPlacedOnMap;
    
    int _bodyDirection;
    int _headDirection;
    
    int _pSpeed;
    int _pHealth;
    int _pArmor;
    int _pShots;
    int _pAmmo;
    
    int _pFuel;
    int _pRawMat;
    int _pGold;
    
    bool _landed;
    bool _detected[MAX_PLAYERS];
    bool IsDetectedByPlayer(unsigned int playerId);
    
    GameUnitData(GameUnitParameters* params, int ownerId);
    ~GameUnitData();
    
    bool GetIsSurvivor() const;
    bool GetIsBuilding() const;
    int GetSize() const;
    bool GetIsAmphibious() const;
    bool GetIsUnderwater() const;
    bool GetIsBuldozer() const;
    bool GetIsConnectored() const;
    bool GetCanStartBuildProcess() const;
    
    vector<UNIT_MENU_ACTION> GetActionList(bool havePath) const;
    SOUND_TYPE GetOnSelectSoundType() const;
    UNIT_SOUND GetBackgroundSoundType() const;
    
    int GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const;
    int GetMaxParameterValue(UNIT_PARAMETER_TYPE parameterType) const;
    void SetParameterValue(UNIT_PARAMETER_TYPE parameterType, int newValue);
    
    void StartNewTurn();
    int GetMoveBalance();
    void MoveWithCost(const int cost);
    int GetShotBalance();
    void MakeShot();
    
    void SetTask(GUTask *newTask);
    void AbortTask();
    void PauseTask();
    void ContinuePausedTask();
    void CompletlyFinishTask();
    
    //returns yes when its ready but not completle, like wait for user finish
    bool GetIsTaskFinished() const;
    bool GetIsTaskPaused() const;
    bool GetIsTaskWaitForUserFinish();
    
    bool ContainsCurrentTask() const {return _currentTask != NULL;}
    GameUnit* GetTaskSecondUnit();
    bool CanMove() const;
    MAXObjectConfig* GetConfig() const;
    bool IsCellOfUnit(const CCPoint &p) const;
    
    bool ReceiveDamage(GameUnitData* unit, int decrase);
    
#pragma mark - Raduis and BBs
    BoundingBox GetBoundingBox(const CCPoint &point, const float radius) const;
    BoundingBox GetCurrentBoundingBox(const float radius) const;
    bool IsInRadius(const CCPoint &point, const float radius) const;
    bool IsInRadius(const CCPoint &point, const float radius, const CCPoint &currentCenter) const;
    
    
    BoundingBox GetScanBoundingBox(const CCPoint &centerPoint) const;
    BoundingBox GetScanBoundingBox() const;
    BoundingBox GetFireBoundingBox() const;
    bool IsInScanRadius(const CCPoint &point) const;
    bool IsInScanRadius(const CCPoint &point, const CCPoint &currentCenter) const;
    bool IsInFireRadius(const CCPoint &point) const;
    bool IsInFireRadius(const CCPoint &point, const CCPoint &currentCenter) const;
    
};

#endif /* defined(__MAX__GameUnitData__) */
