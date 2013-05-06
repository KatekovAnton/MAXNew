//
//  Unit.h
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Unit__
#define __MAX__Unit__

#include <iostream>
#include "GameObject.h"
#include "MAXAnimationDelegate.h"
#include "MAXUnitObjectDelegate.h"
#include "miniPrefix.h"

using namespace std;
using namespace cocos2d;


class MAXUnitObject;
class MAXAnimationBase;
class MAXAnimationSequence;
class MAXAnimationObjectUnit;
class MAXObjectConfig;


class GameUnitCurrentState;
class GameUnitParameters;
class GameUnitDelegate;
class GameMatchPlayer;
class GameEffect;
class PFWaveCell;


class GameUnit : public GameObject, public MAXAnimationDelegate, public MAXUnitObjectDelegate {
    
    MAXAnimationSequence* _currentTopAnimation;
    GameEffect* _effectUnder;
    
    bool _shouldAnimateBody;
    bool _disabledByInfiltrator;
    bool _isInProcess;
    bool _isPlacedOnMap;
    
    std::vector<PFWaveCell*> movePath;
    int pathIndex;
    bool pathIsTemp;
    
    bool MoveToNextCell(void);
    void FollowPath(void);
    
    void ChackForAnimanteBody();
    
public:
    
    bool GetIsFreezed() const { return _currentTopAnimation != NULL; }
    
    GameUnitCurrentState    *_unitCurrentParameters;
    GameUnitDelegate        *_delegate_w;
    GameMatchPlayer         *_owner_w;
    bool _detected;
    MAXUnitObject* GetUnitObject() const {return (MAXUnitObject*) GetObject(); };
    
    GameUnit(MAXUnitObject* unitObject, GameUnitParameters* config);
    ~GameUnit();
    
    
    void CheckBodyAndShadow();
    void SetUnitLocationAnimated(const CCPoint& destination);
    void SetPath(std::vector<PFWaveCell*> path);
    void ClearTempPath();
	std::vector<PFWaveCell*> GetPath();
    int GetPathIndex();
	bool IsPathTargetedTo(const int x, const int y);
	void ConfirmCurrentPath();
    
    void SetDirection(int dir);
    void SetRandomDirection();
    void SetColor(GLKVector4 color);
    
    virtual void SetLocation(const CCPoint& cell);
    virtual void Show();
    virtual void Hide();
	void LiftBridge();
	void DropBridge();
    void TakeOff();
    void Landing();
    void LiftBridgeInstantly();
    void LandInstantly();
    
    //logic methods
    void NewTurn();
    void PlaceUnitOnMap();
    void RemoveUnitFromMap();
    
    void CheckMovementUpdate();
    
    std::vector<UNIT_MENU_ACTION> GetActionList() const;
    MAXObjectConfig* GetBaseConfig();
    int GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const;
    int GetParameterMaxValue(UNIT_PARAMETER_TYPE parameterType) const;
    
    bool GetIsConnectored()const;
    void UpdateConnectors();
    
    
    vector<CCPoint> GetNerbyCells() const;
    
#pragma mark - Fire methods
    bool IsInProcess() const {return _isInProcess;}
    bool CanFire(const cocos2d::CCPoint &target);
    GameEffect* Fire(const CCPoint& target);
    
    
#pragma mark - Build methods
    bool CanStartBuildProcess();
    void StartBuildProcess();
    
    
#pragma mark - MAXAnimationDelegate
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
    
#pragma mark - MAXUnitObjectDelegate
    virtual int GetScan() const;
    virtual int GetRange() const;
    virtual float GetHealStatus() const;
    virtual float GetShots() const;
    virtual bool ShouldAnimateBody() const;
	virtual bool ShoudDrawFakeCircle() const;
	virtual CCPoint GetFakeCenter() const;
    
};

#endif /* defined(__MAX__Unit__) */
