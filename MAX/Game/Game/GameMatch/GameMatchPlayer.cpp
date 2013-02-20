//
//  GameМatchPlayer.cpp
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameMatchPlayer.h"
#include "Texture.h"
#include "PlayerBase.h"
#include "GameUnit.h"
#include "MAXEngine.h"
#include "Texture.h"
#include "MAXContetnLoader.h"
#include "MAXConfigManager.h"

GameMatchPlayer::GameMatchPlayer(GameMatchPlayerInfo info)
{
    _playerInfo = info;
    _palette = MAXSCL->TexturePalleteFormDefaultPalleteAndPlayerColor(_playerInfo._color);
    
}

GameMatchPlayer::~GameMatchPlayer()
{
    delete _palette;
    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        delete unit;
    }
}

GameUnit* GameMatchPlayer::CreateUnit (int posx, int posy, string type, unsigned int ID)
{
    MAXUnitConfig* unit = MAXConfigManager::SharedMAXConfigManager()->GetConfig(type);
    GameUnit* result = new GameUnit(MAXSCL->CreateUnit(unit), unit, this);
    result->SetUnitLocation(CCPoint(posx, posy), false);
    _units.addObject(result);
    return result;
}

GameUnit* GameMatchPlayer::GetUnitInPosition(CCPoint pos)
{
    CCPoint cell;
    for (int i = 0; i < _units.GetCount(); i++) {
        GameUnit* unit = _units.objectAtIndex(i);
        cell = unit->GetUnitCell();
        if ((int)cell.x == (int)pos.x && (int)cell.y == (int)pos.y ) {
            return unit;
        }
    }
    return NULL;
}
