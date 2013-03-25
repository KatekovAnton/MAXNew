//
//  GameMatchPlayer.h
//  MAX
//
//  Created by  Developer on 11.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Game_MatchPlayer__
#define __MAX__Game_MatchPlayer__

#include <iostream>
#include "miniPrefix.h"
#include "MAXContentConstants.h"
#include "PlayerResearchManager.h"

class Texture;
class GameUnit;
class MAXClanConfig;
class PlayerBase;

class PlayerResearchManager;
class PlayerUpgradeManager;

class GameMatch;
class GameFog;

struct GameMatchPlayerInfo {
    
	int clan;
    
    
    unsigned int    _playerId;
    string          _name;
    Color           _color;
};

class GameMatchPlayer {
    
public:
    
    GameMatch *_match_w;
    
    int researchCentersWorkingOnArea[kNrResearchAreas]; ///< counts the number of research centers that are currently working on each area
    
    PlayerResearchManager* _researchManager;
    PlayerUpgradeManager* _upgradeManager;
    
    MAXClanConfig* _clanConfig;
    
    GameMatchPlayerInfo _playerInfo;
    GameFog* _fog;
    
    vector<Texture*> _palettes;
    Texture* _palette;
    
    USimpleContainer<GameUnit*> _units;
    PlayerBase* _base;
    
    GameMatchPlayer(GameMatchPlayerInfo playerInfo, GameMatch *match);
    ~GameMatchPlayer();

    void SetPalette(double time);
    
    void UpdateFogForUnit(GameUnit* unit, const CCPoint &unitPosition);
    void ResetFogForUnit(GameUnit* unit, const CCPoint &unitPosition);
    
    GameUnit* CreateUnit (int posx, int posy, string type, unsigned int ID);
    GameUnit* GetUnitInPosition(const CCPoint& pos);
};

#endif /* defined(__MAX__Game_MatchPlayer__) */
