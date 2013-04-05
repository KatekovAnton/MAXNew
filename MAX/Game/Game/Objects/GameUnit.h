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
#include "MAXStatusRendererUnitDelegate.h"
#include "miniPrefix.h"

using namespace std;
using namespace cocos2d;


class MAXUnitObject;
class MAXAnimationBase;
class MAXAnimationObjectUnit;
class MAXObjectConfig;


class GameUnitCurrentState;
class GameUnitParameters;
class GameUnitDelegate;
class GameMatchPlayer;
class GameEffect;
class PFWaveCell;


class GameUnit : public GameObject, public MAXAnimationDelegate, public MAXStatusRendererUnitDelegate {
    
    MAXAnimationBase* _currentTopAnimation;
    GameEffect* _effectUnder;
    
    bool _isInProcess;
    bool _isPlacedOnMap;
    
    std::vector<PFWaveCell*> movePath;
    int pathIndex;
    
    bool MoveToNextCell(void);
    void FollowPath(void);
    
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
    
    void LowerPlane();
    void LiftPlane();
    void SetDirection(int dir);
    void SetRandomDirection();
    void SetColor(GLKVector4 color);
    
    virtual void SetLocation(const CCPoint& cell);
    virtual void Show();
    virtual void Hide();
    
    //logic methods
    void PlaceUnitOnMap();
    void RemoveUnitFromMap();
    
    void CheckMovementUpdate();
    
    
#pragma mark - Fire methods
    bool IsInProcess() const {return _isInProcess;}
    bool CanFire(const cocos2d::CCPoint &target);
    void Fire(const CCPoint& target);
    
    
#pragma mark - Build methods
    bool CanStartBuildProcess();
    void StartBuildProcess();
    
    
#pragma mark - MAXAnimationDelegate
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
    
#pragma mark - MAXStatusRendererUnitDelegate
    virtual int GetScan() const;
    virtual int GetRange() const;
    virtual float GetHealStatus() const;
    virtual float GetShots() const;
};

#endif /* defined(__MAX__Unit__) */
