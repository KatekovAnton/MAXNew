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
    
    
public:
    
    GameMatchPlayer *_owner_w;
    GameUnitParameters* _config;
    
    bool _detected;
    MAXUnitObject* GetUnitObject() const {return (MAXUnitObject*) GetObject(); };
    
    GameUnit(MAXUnitObject* unitObject, GameUnitParameters* config, GameMatchPlayer* owner);
    ~GameUnit();
    
    
    void CheckBodyAndShadow();
    void SetUnitLocationAnimated(const CCPoint& destination);
    
    void LowerPlane();
    void LiftPlane();
    void Fire(const CCPoint& target);
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
