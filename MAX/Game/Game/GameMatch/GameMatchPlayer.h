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
#include "GameFogDelegate.h"

class Texture;
class GameUnit;
class MAXClanConfig;
class PlayerBase;

class PlayerResearchManager;
class PlayerUpgradeManager;
class PlayerResourceMap;

class GameMatch;
class GameFog;

struct GameMatchPlayerInfo {
    
	int clan;
    
    
    unsigned int    _playerId;
    string          _name;
    Color           _color;
};

class GameMatchPlayer : public GameFogDelegate {
    
    vector<Texture*> _palettes;
    Texture* _palette;
    
public:
    
    bool GetIsCurrentPlayer() const;
    Texture** GetPalettePointer() {return &_palette;};
    
    GameMatch *_match_w;
    
    int researchCentersWorkingOnArea[kNrResearchAreas]; ///< counts the number of research centers that are currently working on each area
    
    PlayerResearchManager   *_researchManager;
    PlayerUpgradeManager    *_upgradeManager;
    PlayerResourceMap       *_resourceMap;
    
    MAXClanConfig* _clanConfig;
    CCPoint _landingPosition;
    
    GameMatchPlayerInfo _playerInfo;
    GameFog* _fog;
    GameFog* _resourceMapFog;

    
    USimpleContainer<GameUnit*> _units;
    PlayerBase* _base;
    
    GameMatchPlayer(GameMatchPlayerInfo playerInfo, GameMatch *match);
    ~GameMatchPlayer();

    void SetPalette(double time);
    
    void LandingTo(const CCPoint &landingPosition);
    
    void UnitDidMove(GameUnit *unit, const CCPoint &oldPoint, const CCPoint &newPoint);
    void UnitDidPlaceToMap(GameUnit* unit);
    void UnitDidRemoveFromMap(GameUnit* unit);
    
    GameUnit* CreateUnit (int posx, int posy, string type, unsigned int ID);
    GameUnit* GetUnitInPosition(const CCPoint& pos);
    
#pragma mark - GameFogDelegate

    virtual bool UnitShouldUpdateFog(const GameUnit *unit, const GameFog *fog) const;
    virtual float UnitScanRadiusForFog(const GameUnit *unit, const GameFog *fog) const;
    virtual void CellDidUpdate(const int cellX, const int cellY, const GameFog *fog, bool visibleFlag) const;
    
};

#endif /* defined(__MAX__Game_MatchPlayer__) */
