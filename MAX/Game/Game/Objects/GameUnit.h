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
#include "GameEffectDelegate.h"

using namespace std;
using namespace cocos2d;


class MAXUnitObject;
class MAXAnimationBase;
class MAXAnimationSequence;
class MAXAnimationObjectUnit;
class MAXObjectConfig;


class GameUnitData;
class GameUnitParameters;
class GameUnitDelegate;
class GameMatchPlayer;
class GameEffect;
class PFWaveCell;


class GameUnit : public GameObject, public MAXAnimationDelegate, public MAXUnitObjectDelegate, public GameEffectDelegate
{
    
    MAXAnimationSequence* _currentTopAnimation;
    MAXAnimationBase*   _removeDelayAnim;
    GameEffect* _effectUnder;
    GameEffect* _effectOver;
    
    bool _shouldAnimateBody;
    int _currentSound;
    
    std::vector<PFWaveCell*> movePath;
    int pathIndex;
    int movePathIndex;
    bool pathIsTemp;
    
    bool MoveToNextCell(void);
    void FollowPath(void);
    
    void CheckForAnimanteBody();
    void SetUnitLocationAnimated(const CCPoint& destination);
    
public:
    
    //just temporary flag
    bool _currentlyProcesedConstructor;
    
    vector<GameUnit*>       _storedUnits_w;
    GameUnitData            *_unitData;
    GameUnitDelegate        *_delegate_w;
    GameMatchPlayer         *_owner_w;
    
    
    GameEffect* GetEffectUnder() { return _effectUnder; }
    bool GetIsFreezed() const { return _currentTopAnimation != NULL; }
    bool GetIsStealthable() const;
    MAXUnitObject* GetUnitObject() const { return (MAXUnitObject*) GetObject(); };
    std::vector<UNIT_MENU_ACTION> GetActionList() const;
    MAXObjectConfig* GetConfig();
    int GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const;
    int GetParameterMaxValue(UNIT_PARAMETER_TYPE parameterType) const;
    vector<CCPoint> GetNerbyCells() const;
    vector<CCPoint> GetFullNearbyCells() const;
    
    bool ReceiveDamage(GameUnit* unit);
    bool ReceiveDamage(GameUnit* unit, int decrase);
    
    GameUnit(MAXUnitObject* unitObject, GameUnitParameters* params, int playerId, GameMatchPlayer *owner);      //creates new unit
    GameUnit(MAXUnitObject* unitObject, GameUnitData* unitdata, GameMatchPlayer *owner);                        //creates unit from saved data
    void Init();
    ~GameUnit();
    
    void RemoveWithDelay(double delay);
    void RunAnimation(MAXAnimationSequence* animation);
    
    int PlaySound(UNIT_SOUND unitSound);
    void StopCurrentSound();
    void UnitDidSelect();
    void UnitDidDeselect();
    void CheckBodyAndShadow();
    void CheckBuildProcess();
    
    void SetPath(std::vector<PFWaveCell*> path);
	void ClearPath();
    void ClearTempPath();
	std::vector<PFWaveCell*> GetPath();
    int GetPathIndex();
    PFWaveCell* GetNextPathCell();
	bool IsPathTargetedTo(const int x, const int y);
	void ConfirmCurrentPath();
    void AbortCurrentPath();
    
    void SetDirection(int dir);
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
    void EscapeToLocation(const int x, const int y, const int cost);
    
    //logic methods
    void NewTurn();
    void PlaceUnitOnMap();
    void RemoveUnitFromMap();
    
    void Destroy();
    
    bool CanMove() const;
    
    void CheckMovementUpdate();
    
    void UpdateConnectors();
    void DetectedByPlayer(unsigned int playerId);
    bool IsDetectedByPlayer(unsigned int playerId);
    
#pragma mark - Fire methods
    bool CanFire(const cocos2d::CCPoint &target);
    GameEffect* MakeWeaponAnimationEffect(const cocos2d::CCPoint &target);
    void Fire(const CCPoint& target, const int level);
    CCPoint fireTarget;
    
#pragma mark - Build methods
    void CreateCheckIcon();
    void DestroyCheckIcon();
    
    void CreateSmallBuildingTape();
    void CreateLargeBuildingTape();
    void DestroyBuildingTape();
    
    void StartConstructingUnitInPlace(const CCPoint &topLeftCell, MAXObjectConfig *buildingConfig);
    void PauseConstructingUnit();
    void CancelConstructingUnit();
    void EscapeConstructedUnit(const CCPoint &cell);
    void AbortConstructingUnit();
    
    void BeginConstructionSequence();
    void EndConstructionSequense();

    void TurnOn();
    void TurnOf();
    
    
#pragma mark - MAXAnimationDelegate
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
    
#pragma mark - MAXUnitObjectDelegate
    virtual bool ShouldSkipThisUnit() const;
    virtual int GetScan() const;
    virtual int GetRange() const;
    virtual float GetHealStatus() const;
    virtual float GetShots() const;
    virtual bool ShouldAnimateBody() const;
	virtual bool ShoudDrawFakeCircle() const;
	virtual CCPoint GetFakeCenter() const;
    
    
#pragma mark - GameEffectDelegate
    virtual void GameEffectDidStartExistance(GameEffect* effect);
    virtual void GameEffectDidFinishExistance(GameEffect* effect);
    
};

#endif /* defined(__MAX__Unit__) */
