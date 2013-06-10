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
#include "MAXContentLoader.h"
#include "MAXAnimationPrefix.h"
#include "MAXGame.h"
#include "GameUnit.h"
#include "GameUnitBaseParameters.h"
#include "GameUnitParameters.h"

//-подложка под большое здание                                                  LRGSLAB	mult
//-подложка под маленькое здание                                                SMLSLAB	mult
//-строительная подложка большая наземная                                       LRGTAPE	mult
//-строительная подложка большая морская                                        LRGCONES	mult
//-строительная подложка маленькая наземная                                     SMLTAPE
//-строительная подложка маленькая морская                                      SMLCONES

//-анимированные стрелки которые указывают на место для вывода юнита из здания	BLDMRK1-8	?
//-мусор на 1 клетку                                                            SMLRUBLE	mult
//-мусор на 4 клетки                                                            LRGRUBLE	mult

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

//ВОТ АЛЕКСЕЙ какие костыли приходится городить чтоб сделать мусор юнитом
MAXObjectConfig* trash1x1config = NULL;
MAXObjectConfig* trash2x2config = NULL;
//GameUnitBaseParameters* trash1x1params = NULL;
//GameUnitBaseParameters* trash2x2params = NULL;

GameEffect::GameEffect(MAXEffectObject* effectObject, MAXObjectConfig* config, bool addToEffectList)
:GameObject(effectObject, config), _config(config), _finished(false), _blastType(BLAST_TYPE_NONE), _secondaryType(SECONDARY_TYPE_NONE), _lastSmokeCreationTime(engine->FullTime()), _delegate_w(NULL)
{
    if (addToEffectList)
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

void GameEffect::Show()
{
    GameObject::Show();
    
    if (_effectType == EFFECT_TYPE_BULLET)
    {
        _lastSmokeCreationPosition = GetObject()->GetObjectCell();
        _rangeToTarget = sqrtf(powf(_targetCell.x - _lastSmokeCreationPosition.x, 2) + powf(_targetCell.y - _lastSmokeCreationPosition.y, 2));
        _startCell = _lastSmokeCreationPosition;
    }
}

void GameEffect::Hide()
{
    GameObject::Hide();
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
    config->_bSize = size;
    config->_isBuilding = false;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, true);
    GameEffect* result = new GameEffect(effectObject, config, true);
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
    config->_bSize = size;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, animated);
    GameEffect* result = new GameEffect(effectObject, config, true);
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
    config->_bSize = size;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, true);
    GameEffect* result = new GameEffect(effectObject, config, true);
    result->_effectType = EFFECT_TYPE_SECONDARY;
    return result;
}

GameEffect* GameEffect::CreateTrash(TRASH_TYPE type)
{
    MAXObjectConfig* config = NULL;
    
    if (type == TRASH_TYPE_SMALL)
    {
        if (!trash1x1config) {
            trash1x1config = new MAXObjectConfig();
            trash1x1config->_bLevel = OBJECT_LEVEL_ONGROUND;
            trash1x1config->_bodyName = "SMLRUBLE";
            trash1x1config->_shadowName = "";
            trash1x1config->_bSize = 1;
            trash1x1config->_isBuilding = true;
            
           // trash1x1params = new GameUnitBaseParameters(trash1x1config);
        }
        config = trash1x1config;
        
    }
    else
    {
        if (!trash2x2config) {
            trash2x2config = new MAXObjectConfig();
            trash2x2config->_bLevel = OBJECT_LEVEL_ONGROUND;
            trash2x2config->_bodyName = "LRGRUBLE";
            trash2x2config->_shadowName = "";
            trash2x2config->_bSize = 2;
            trash2x2config->_isBuilding = true;
            
            //trash2x2params = new GameUnitBaseParameters(trash2x2config);
        }
        config = trash2x2config;
    }
    
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, config->_bSize, false);// CreateUnit(config->_configObject);
    GameEffect* result = new GameEffect(effectObject, config, true);
    result->_effectType = EFFECT_TYPE_TRASH;
    //GameUnitParameters* params = new GameUnitParameters(config);
    //GameUnit* result = new GameUnit(object, params);
    //result->SetColor(GLKVector4Make(0, 0, 0, 0));
    //result->_owner_w = NULL;
    
    return result;
}

GameEffect* GameEffect::CreateBuildingBase(BUILDING_BASE_TYPE type, int level)
{
    string effectName = "";
    float size = 1.0;
    switch (type) {
        case BUILDING_BASE_TYPE_LARGE:
        {
            size = 2.0;
            effectName = "LRGSLAB";
        }   break;
        case BUILDING_BASE_TYPE_SMALL:
        {
            size = 1.0;
            effectName = "SMLSLAB";
        }   break;
        case BUILDING_BASE_TYPE_PROGRESS_LARGE:
        {
            size = 2.0;
            effectName = "LRGTAPE";
        }   break;
        case BUILDING_BASE_TYPE_PROGRESS_SEA_LARGE:
        {
            size = 2.0;
            effectName = "LRGCONES";
        }   break;
        case BUILDING_BASE_TYPE_PROGRESS_SMALL:
        {
            size = 1.0;
            effectName = "SMLTAPE";
        }   break;
        case BUILDING_BASE_TYPE_PROGRESS_SEA_SMALL:
        {
            size = 1.0;
            effectName = "SMLCONES";
        }   break;
        default:
            break;
    }
    
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = effectName;
    config->_bSize = size;
    config->_isBuilding = true;
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, size, false);
    GameEffect* result = new GameEffect(effectObject, config, false);
    result->_effectType = EFFECT_TYPE_BUILDING_BASE;
    result->_blastType = BLAST_TYPE_NONE;
    result->_secondaryType = SECONDARY_TYPE_NONE;
    return result;
}

GameEffect* GameEffect::CreatePathArrow(int azimut, bool isGreen, int level)
{
    string effectName = "arrow_up.png";
    float size = 1.0;
    
    switch (azimut%16) {
        case 0:
            effectName = "arrow_up_png_2.png";
            break;
        case 1:
            effectName = "arrow_up_right_png_2.png";
            break;
        case 2:
            effectName = "arrow_right_png_2.png";
            break;
        case 3:
            effectName = "arrow_right_down_png_2.png";
            break;
        case 4:
            effectName = "arrow_down_png_2.png";
            break;
        case 5:
            effectName = "arrow_left_down_png_2.png";
            break;
        case 6:
            effectName = "arrow_left_png_2.png";
            break;
        case 7:
            effectName = "arrow_left_top_png_2.png";
            break;
        case 8:
            effectName = "arrow_up_png.png";
            break;
        case 9:
            effectName = "arrow_up_right_png.png";
            break;
        case 10:
            effectName = "arrow_right_png.png";
            break;
        case 11:
            effectName = "arrow_right_down_png.png";
            break;
        case 12:
            effectName = "arrow_down_png.png";
            break;
        case 13:
            effectName = "arrow_left_down_png.png";
            break;
        case 14:
            effectName = "arrow_left_png.png";
            break;
        case 15:
            effectName = "arrow_left_top_png.png";
            break;
            
        default:
            break;
    }
    
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = effectName;
    config->_bSize = size;
    config->_isBuilding = true;
    MAXEffectObject* effectObject = MAXSCL->CreateSingleEffect(config, size);
    effectObject->_currentFrame=0;
    effectObject->_drawInLowLod = true;
    GameEffect* result = new GameEffect(effectObject, config, false);
    result->_effectType = EFFECT_TYPE_BUILDING_BASE;
    result->_blastType = BLAST_TYPE_NONE;
    result->_secondaryType = SECONDARY_TYPE_NONE;
    return result;
}

GameEffect* GameEffect::CreateTaskCompletedChack(int level)
{
    string effectName = "glk.png";
    float size = 1.0;
    
        
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = effectName;
    config->_bSize = size;
    config->_isBuilding = true;
    MAXEffectObject* effectObject = MAXSCL->CreateSingleEffect(config, size);
    effectObject->_currentFrame=0;
    effectObject->_drawInLowLod = true;
    GameEffect* result = new GameEffect(effectObject, config, false);
    result->_effectType = EFFECT_TYPE_BUILDING_BASE;
    result->_blastType = BLAST_TYPE_NONE;
    result->_secondaryType = SECONDARY_TYPE_NONE;
    return result;
}

GameEffect* GameEffect::CreateExitPlaceMarker(int level)
{
    MAXObjectConfig* config = new MAXObjectConfig();
    config->_bLevel = level;
    config->_bodyName = "";
    config->_bSize = 1;
    config->_isBuilding = true;
    vector<string> names;
    names.push_back("BLDMRK1");
    names.push_back("BLDMRK2");
    names.push_back("BLDMRK3");
    names.push_back("BLDMRK4");
    names.push_back("BLDMRK5");
    MAXEffectObject* effectObject = MAXSCL->CreateEffect(config, 1, true, names);
    effectObject->_currentFrame=0;
    effectObject->_drawInLowLod = true;
    GameEffect* result = new GameEffect(effectObject, config, false);
    result->_effectType = EFFECT_TYPE_BUILDING_BASE;
    result->_blastType = BLAST_TYPE_NONE;
    result->_secondaryType = SECONDARY_TYPE_NONE;
    return result;
}

#pragma mark - MAXAnimationDelegate

void GameEffect::OnAnimationStart(MAXAnimationBase* animation)
{
    switch (_effectType) {
        case EFFECT_TYPE_BULLET:
        {
        }   break;
        case EFFECT_TYPE_BLAST:
        {
            if (_delegate_w)
                _delegate_w->GameEffectDidFinishExistance(this);
            
            
        }   break;
        default:
            break;
    }
    
}

void GameEffect::OnAnimationUpdate(MAXAnimationBase* animation)
{
    if (_secondaryType != SECONDARY_TYPE_NONE && _lastSmokeCreationTime + 0.005 < engine->FullTime())
    {
        CCPoint currentPosition = GetObject()->GetObjectCell();
        float minDelta = 0.3;
        
        float range = sqrtf(powf(currentPosition.x - _lastSmokeCreationPosition.x, 2) + powf(currentPosition.y - _lastSmokeCreationPosition.y, 2));
        int times = range/minDelta;
        CCPoint direction = ccp(currentPosition.x - _lastSmokeCreationPosition.x, currentPosition.y - _lastSmokeCreationPosition.y);
        direction.x /= range;
        direction.y /= range;
        for (int i = 0; i < times; i++)
        {
            _lastSmokeCreationPosition.x += direction.x * minDelta;
            _lastSmokeCreationPosition.y += direction.y * minDelta;
            
            float thisRange = sqrtf(powf(_lastSmokeCreationPosition.x - _startCell.x, 2) + powf(_lastSmokeCreationPosition.y - _startCell.y, 2));
            if (thisRange > _rangeToTarget) 
                break;
            
            
            GameEffect* blast = GameEffect::CreateSecondaryEffect(_secondaryType, _config->_bLevel);
            blast->SetLocation(_lastSmokeCreationPosition);
            blast->Show();
            MAXAnimationWait* wait = new MAXAnimationWait(blast->GetFrameCount() * 0.1);
            wait->_delegate = blast;
            MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
            
        }
        
        _lastSmokeCreationTime = engine->FullTime();
    }
}

void GameEffect::OnAnimationFinish(MAXAnimationBase* animation)
{
    switch (_effectType) {
        case EFFECT_TYPE_BULLET:
        {
            GameObject::Hide();
            game->FlushEffectsWithNew(this);
            _finished = true;
            if (_blastType != BLAST_TYPE_NONE)
            {
                GameEffect* blast = GameEffect::CreateBlast(_blastType, _config->_bLevel);
                blast->SetLocation(GetObject()->GetObjectCell());
                blast->Show();
                MAXAnimationWait* wait = new MAXAnimationWait(blast->GetFrameCount() * 0.1);
                wait->_delegate = blast;
                MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(wait);
                blast->_delegate_w = _delegate_w;
                blast->_tag = _tag;
                this->_delegate_w = NULL;
            }
            else
            {
                if (_delegate_w)
                    _delegate_w->GameEffectDidFinishExistance(this);
            }
        }   break;
        case EFFECT_TYPE_BLAST:
        {
            GameObject::Hide();
            game->FlushEffectsWithNew(this);
            _finished = true;
            if (_delegate_w) 
                _delegate_w->GameEffectDidFinishExistance(this);
            
            
        }   break;
        case EFFECT_TYPE_SECONDARY:
        {
            GameObject::Hide();
            game->FlushEffectsWithNew(this);
            _finished = true;
        }   break;
            
        default:
            break;
    }
}


