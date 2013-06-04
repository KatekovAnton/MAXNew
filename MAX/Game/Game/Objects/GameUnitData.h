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
    
    bool _isInProcess;
    GUTask *_currentTask;
    bool _paused;
    
public:
    
    
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
    
    GameUnitData(GameUnitParameters* params);
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
    
    int GetParameterValue(UNIT_PARAMETER_TYPE parameterType);
    int GetMaxParameterValue(UNIT_PARAMETER_TYPE parameterType);
    void SetParameterValue(UNIT_PARAMETER_TYPE parameterType, int newValue);
    
    void StartNewTurn();
    int GetMoveBalance();
    void MoveWithCost(const int cost);
    int GetShotBalance();
    void MakeShot();
    
    bool GetIsInProcess() const {return _isInProcess;}
    
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
};

#endif /* defined(__MAX__GameUnitData__) */
