//
//  GameEffect.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameEffect.h"
#include "MAXEffectObject.h"
#include "MAXContetnLoader.h"

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
//залупленный многоразовый эфект для например маркеров выхода,
//либо такйже точно залупленный но одноразовый для взрывов и дыма,
//либо просто статика, как для мусора, стрелок пути и оградок вокруг строителей-быльдозеров

GameEffect::GameEffect(MAXEffectObject* effectObject, MAXObjectConfig* config)
:GameObject(effectObject), _config(config), _finished(false)
{}

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

GameEffect* GameEffect::CreateBlast(BLAST_TYPE type)
{
    return NULL;
}

GameEffect* GameEffect::CreateBullet(BULLET_TYPE type, int level)
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
    
    return result;
}

GameEffect* GameEffect::CreateSecondaryEffect(SECONDARY_TYPE type)
{
    return NULL;
}

GameEffect* GameEffect::CreateTrash(TRASH_TYPE type)
{
    return NULL;
}

GameEffect* GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE type)
{
    return NULL;
}

#pragma mark - MAXAnimationDelegate

void GameEffect::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameEffect::OnAnimationUpdate(MAXAnimationBase* animation)
{}

void GameEffect::OnAnimationFinish(MAXAnimationBase* animation)
{
    GameObject::RemoveFromMap();
    game->FlushEffectsWithNew(this);
    _finished = true;
}



