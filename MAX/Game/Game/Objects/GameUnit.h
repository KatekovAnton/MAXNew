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

using namespace std;
using namespace cocos2d;


class MAXUnitObject;
class MAXAnimationBase;
class MAXAnimationObjectUnit;
class MAXUnitConfig;

class GameUnit : public GameObject, public MAXAnimationDelegate {
    
    MAXAnimationObjectUnit* _moveAnimation;
    MAXAnimationBase* _currentTopAnimation;
    MAXUnitConfig* _config;
    CCPoint _unitCell;
    shared_ptr<MAXUnitObject> _unitObject;
    
    
public:
    shared_ptr<MAXUnitObject> GetUnitObject() const {return _unitObject;};
    CCPoint GetUnitCell() const {return _unitCell;};
    
    GameUnit(shared_ptr<MAXUnitObject> unitObject);
    ~GameUnit();
    
    void Fire(const CCPoint& target);
    
    void SetUnitLocation(const CCPoint& cell, const bool animated);
    
    static shared_ptr<GameUnit> CreateUnit(string balanceConfigName, string resourceConfigName);
    
#pragma mark - MAXAnimationDelegate
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
};

#endif /* defined(__MAX__Unit__) */
