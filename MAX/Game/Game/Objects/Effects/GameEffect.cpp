//
//  GameEffect.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameEffect.h"
#include "MAXEffectObject.h"
#include "MAXObjectConfig.h"

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
//-все виды ракет(воздушная наземная итд или там везде один?)                   ROCKET
//-дым от ракеты                                                                RKTSMOKE
//-торпеда                                                                      TORPEDO
//-пузыри от торпеды                                                            TRPBUBLE
//-плазма                                                                       ALNABALL
//-мелкий взрыв(эффект попадания в неуничтоженный объект)                       HITEXPLD
//контроллер реализует поведение эффекта-
//залупленный многоразовый эфект для например маркеров выхода,
//либо такйже точно залупленный но одноразовый для взрывов и дыма,
//либо просто статика, как для мусора, стрелок пути и оградок вокруг строителей-быльдозеров

GameEffect::GameEffect(MAXEffectObject* effectObject, MAXObjectConfig* config)
:GameObject(effectObject)
{}

#pragma mark - creation

GameEffect* CreateBlast(BLAST_TYPE type)
{
    return NULL;
}

GameEffect* CreateBullet(BULLET_TYPE type)
{
    return NULL;
}

GameEffect* CreateSecondaryEffect(SECONDARY_TYPE type)
{
    return NULL;
}

GameEffect* CreateTrash(TRASH_TYPE type)
{
    return NULL;
}

GameEffect* CreateBuildingBase(BUILDING_BASE_TYPE type)
{
    return NULL;
}

#pragma mark - MAXAnimationDelegate

void GameEffect::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameEffect::OnAnimationUpdate(MAXAnimationBase* animation)
{}

void GameEffect::OnAnimationFinish(MAXAnimationBase* animation)
{}