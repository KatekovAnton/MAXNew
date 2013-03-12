//
//  GameEffect.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameEffect.h"
#include "MAXEngine.h"
#include "MAXEffectObject.h"
#include "MAXContetnLoader.h"
#include "MAXAnimationPrefix.h"
#include "MAXGame.h"

//-подложка под большое здание                                                  LRGSLAB	mult
//-подложка под маленькое здание                                                SMLSLAB	mult
//-анимированные стрелки которые указывают на место для вывода юнита из здания	BLDMRK1-8	?
//-мусор на 1 клетку                                                            SMLRUBLE	mult
//-мусор на 4 клетки                                                            LRGRUBLE	mult
//-строительная подложка большая наземная                                       LRGTAPE	mult
//-строительная подложка большая морская                                        LRGCONES	mult
//-строительная подложка маленькая наземная                                     SMLTAPE
//-строительная подложка маленькая морская                                      SMLCONES

//-взрыв в воздухе                                                              AIREXPLD
//-взрыв на море                                                                SEAEXPLD
//-взрыв на земле                                                               LNDEXPLD
//-взрыв от здания                                                              BLDEXPLD
//-мелкий взрыв(эффект попадания в неуничтоженный объект)                       HITEXPLD

//-дым от ракеты                                                                RKTSMOKE
//-пузыри от торпеды                                                            TRPBUBLE

//-торпеда                                                                      TORPEDO
//-все виды ракет(воздушная наземная итд или там везде один?)                   ROCKET
//-плазма                                                                       ALNABALL

//контроллер реализует поведение эффекта-
//зацикленный многоразовый эфект для например маркеров выхода,
//либо такйже точно залупленный но одноразовый для взрывов и дыма,
//либо просто статика, как для мусора, стрелок пути и оградок вокруг строителей-быльдозеров

GameEffect::GameEffect(MAXEffectObject* effectObject, MAXObjectConfig* config)
:GameObject(effectObject), _config(config), _finished(false), _blastType(BLAST_TYPE_NONE), _secondaryType(SECONDARY_TYPE_NONE), _lastSmokeCreationTime(engine->FullTime())
{
    game->_effects->addObject(this);
    _frameCount = effectObject->_frameCount;
}

GameEffect::~GameEffect()
{
    delete _config;
}

void GameEffect::SetDirection(int index)
{
    MAXEffectObject* obj = (MAXEffectObject*)GetObject();
    obj->_currentFrame = index;
}

#pragma mark - creation

GameEffect* GameEffect::CreateBlast(BLAST_TYPE type, int level)
{
    string effectName = "AIREXPLD";
    float size = 1.0;
    switch (type)
    {
        case BLAST_TYPE_AIR:
            effectName = "AIREXPLD";
          break;
        case BLAST_TYPE_BUILDING:
        {
            effectName = "BLDEXPLD";
            size = 2.0;
        }   break;
        case BLAST_TYPE_DAMAGEEFFECT:
            effectName = "HITEXPLD";
            break;
        case BLAST_TYPE_GROUND:
            effectName = "LNDEXPLD";
            break;
        case BLAST_TYPE_SEA:
            effectName = "SEAEXPLD";
            break;
        default:
            break;
    }
    
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = effectName;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, true);
    GameEffect* result = new GameEffect(effectObject, config);
    result->_effectType = EFFECT_TYPE_BLAST;
    return result;
}

GameEffect* GameEffect::CreateBullet(BULLET_TYPE type, int level, BLAST_TYPE blastType, SECONDARY_TYPE secondarytype)
{
    string effectName = "ALNABALL";
    bool animated = true;
    float size = 1.0;
    switch (type) {
        case BULLET_TYPE_ROCKET:
        {
            animated = false;
            effectName = "ROCKET";
        }   break;
        case BULLET_TYPE_TORPEDO:
        {
            animated = false;
            effectName = "TORPEDO";
        }   break;
        default:
            break;
    }
    
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = effectName;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, animated);
    GameEffect* result = new GameEffect(effectObject, config);
    result->_effectType = EFFECT_TYPE_BULLET;
    result->_blastType = blastType;
    result->_secondaryType = secondarytype;
    return result;
}

GameEffect* GameEffect::CreateSecondaryEffect(SECONDARY_TYPE type, int level)
{
    string effectName = "RKTSMOKE";
    float size = 1.0;
    switch (type)
    {
        case SECONDARY_TYPE_RIBBLES:
            effectName = "TRPBUBLE";
            break;
        default:
            break;
    }
    
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = effectName;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, true);
    GameEffect* result = new GameEffect(effectObject, config);
    result->_effectType = EFFECT_TYPE_SECONDARY;
    return result;
}

GameEffect* GameEffect::CreateTrash(TRASH_TYPE type, int level)
{
    return NULL;
}

GameEffect* GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE type, int level)
{
    return NULL;
}

#pragma mark - MAXAnimationDelegate

void GameEffect::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameEffect::OnAnimationUpdate(MAXAnimationBase* animation)
{
    if (_secondaryType != SECONDARY_TYPE_NONE && _lastSmokeCreationTime + 0.005 < engine->FullTime())
    {
        _lastSmokeCreationTime = engine->FullTime();
        GameEffect* blast = GameEffect::CreateSecondaryEffect(_secondaryType, _config->_bLevel);
        blast->SetLocation(GetObject()->ObjectCell());
        blast->LocateOnMap();
        MAXAnimationWait* wait = new MAXAnimationWait(blast->GetFrameCount() * 0.1);
        wait->_delegate = blast;
        MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
    }
}

void GameEffect::OnAnimationFinish(MAXAnimationBase* animation)
{
    switch (_effectType) {
        case EFFECT_TYPE_BULLET:
        {
            GameObject::RemoveFromMap();
            game->FlushEffectsWithNew(this);
            _finished = true;
            if (_blastType != BLAST_TYPE_NONE)
            {
                GameEffect* blast = GameEffect::CreateBlast(_blastType, _config->_bLevel);
                blast->SetLocation(GetObject()->ObjectCell());
                blast->LocateOnMap();
                MAXAnimationWait* wait = new MAXAnimationWait(blast->GetFrameCount() * 0.1);
                wait->_delegate = blast;
                MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
            }
        }   break;
        case EFFECT_TYPE_BLAST:
        case EFFECT_TYPE_SECONDARY:
        {
            GameObject::RemoveFromMap();
            game->FlushEffectsWithNew(this);
            _finished = true;
        }   break;
            
        default:
            break;
    }
}



