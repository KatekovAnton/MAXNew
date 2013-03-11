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

typedef enum
{
    BLAST_TYPE_GROUND,
    BLAST_TYPE_SEA,
    BLAST_TYPE_AIR,
    BLAST_TYPE_BUILDING,
    BLAST_TYPE_DAMAGEEFFECT
} BLAST_TYPE;

typedef enum
{
    BULLET_TYPE_ROCKET,
    BULLET_TYPE_TORPEDO,
    BULLET_TYPE_PLASMA
} BULLET_TYPE;

typedef enum
{
    SECONDARY_TYPE_SMOKE,
    SECONDARY_TYPE_RIBBLES
} SECONDARY_TYPE;

typedef enum
{
    TRASH_TYPE_SMALL,
    TRASH_TYPE_LARGE
} TRASH_TYPE;

typedef enum
{
    BUILDING_BASE_TYPE_LARGE,
    BUILDING_BASE_TYPE_SMALL,
    BUILDING_BASE_TYPE_PROGRESS_SMALL,
    BUILDING_BASE_TYPE_PROGRESS_LARGE,
    BUILDING_BASE_TYPE_PROGRESS_SEA_SMALL,
    BUILDING_BASE_TYPE_PROGRESS_SEA_LARGE
} BUILDING_BASE_TYPE;

class MAXEffectObject;
class MAXObjectConfig;
class MAXAnimationObject;

class GameEffect : public GameObject, public MAXAnimationDelegate {
    
    MAXObjectConfig* _config;
    MAXAnimationObject* _moveAnimation;
    
public:
    
    GameEffect(MAXEffectObject* effectObject, MAXObjectConfig* config);
    ~GameEffect();
     
#pragma mark - creation
    static GameEffect* CreateBlast(BLAST_TYPE type);
    static GameEffect* CreateBullet(BULLET_TYPE type, int level);
    static GameEffect* CreateSecondaryEffect(SECONDARY_TYPE type);
    static GameEffect* CreateTrash(TRASH_TYPE type);
    static GameEffect* CreateBuildingBase(BUILDING_BASE_TYPE type);
    
#pragma mark - MAXAnimationDelegate
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
};

#endif /* defined(__MAX__GameEffect__) */
