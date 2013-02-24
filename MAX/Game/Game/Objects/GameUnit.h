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
#include "cocos2d.h"
#include "MAXAnimationDelegate.h"
#include "MAXStatusRendererUnitDelegate.h"

using namespace std;
using namespace cocos2d;


class MAXUnitObject;
class MAXAnimationBase;
class MAXAnimationObjectUnit;
class MAXUnitConfig;

class GameUnitParameters;
class GameMatchPlayer;

class GameUnit : public GameObject, public MAXAnimationDelegate, public MAXStatusRendererUnitDelegate {
    
    MAXAnimationObjectUnit* _moveAnimation;
    MAXAnimationBase* _currentTopAnimation;
    CCPoint _unitCell;
    MAXUnitObject* _unitObject;
    
    void CheckBodyAndShadow();
    
public:
    
    GameMatchPlayer *_owner_w;
    
    bool _detected;
    bool _onMap;
    
    GameUnitParameters* _config;
    
    MAXUnitObject* GetUnitObject() const {return _unitObject;};
    CCPoint GetUnitCell() const {return _unitCell;};
    
    GameUnit(MAXUnitObject* unitObject, GameUnitParameters* config, GameMatchPlayer* owner);
    ~GameUnit();
    
    void LocateOnMap();
    void RemoveFromMap();
    void LowerPlane();
    void LiftPlane();
    void Fire(const CCPoint& target);
    void SetUnitLocation(const CCPoint& cell, const bool animated);
    void SetDirection(int dir);
    void SetRandomDirection();
    
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
