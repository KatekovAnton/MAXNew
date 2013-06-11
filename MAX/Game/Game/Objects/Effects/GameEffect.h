//
//  GameEffect.h
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameEffect__
#define __MAX__GameEffect__

#include <iostream>
#include "GameObject.h"
#include "MAXAnimationDelegate.h"
#include "MAXObjectConfig.h"
#include "GameEffectDelegate.h"

class MAXEffectObject;
class MAXObjectConfig;
class MAXAnimationObject;

class GameEffect : public GameObject, public MAXAnimationDelegate {
    
    MAXObjectConfig* _config;
    MAXAnimationObject* _moveAnimation;
    
    int _frameCount;
    bool _finished;
    
    EFFECT_TYPE _effectType;
    BULLET_TYPE _bulletType;
    BLAST_TYPE _blastType;
    SECONDARY_TYPE _secondaryType;
    
    double _lastSmokeCreationTime;
    CCPoint _lastSmokeCreationPosition;
public:
    CCPoint _startCell;
    CCPoint _targetCell;
    float _rangeToTarget;
    GameEffectDelegate* _delegate_w;
    
    bool GetFinished() const {return _finished;}
    int GetFrameCount() const {return _frameCount;}
    
    GameEffect(MAXEffectObject* effectObject, MAXObjectConfig* config, bool addToEffectList);
    ~GameEffect();
    
    void SetDirection(int index);
    
    
    virtual void Show();
    virtual void Hide();
     
#pragma mark - creation
    static GameEffect* CreateBlast(BLAST_TYPE type, int level);
    static GameEffect* CreateBullet(BULLET_TYPE type, int level, BLAST_TYPE blastType, SECONDARY_TYPE secondarytype);
    static GameEffect* CreateSecondaryEffect(SECONDARY_TYPE type, int level);
    static GameEffect* CreateTrash(TRASH_TYPE type);
    static GameEffect* CreateBuildingBase(BUILDING_BASE_TYPE type, int level);
    static GameEffect* CreatePathArrow(int azimut, bool isGreen, int level);
    static GameEffect* CreateTaskCompletedChack(int level);
    static GameEffect* CreateExitPlaceMarker(int level);
    
#pragma mark - MAXAnimationDelegate
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
};

#endif /* defined(__MAX__GameEffect__) */
