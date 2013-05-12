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
#include "PlayerResearchManager.h"
#include "GameFogDelegate.h"
#include "GameUnitDelegate.h"

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
    
	int             _clan;
    unsigned int    _playerId;
    string          _name;
    Color           _color;
};

class GameMatchPlayer : public GameFogDelegate, public GameUnitDelegate {
    
    vector<Texture*> _palettes;
    Texture* _palette;
    bool *_resourceCover;
    
    bool UnitShouldUpdateFog(const GameUnit *unit, const GameFog *fog) const;
    bool UnitCoveredByFog(const GameUnit *unit, const GameFog *fog) const;
public:
    
    bool GetIsCurrentPlayer() const;
    Texture** GetPalettePointer() {return &_palette;};
    
    CCPoint cameraPosition;
    
    GameMatch *_match_w;
    
    int researchCentersWorkingOnArea[kNrResearchAreas]; ///< counts the number of research centers that are currently working on each area
    
    PlayerResearchManager   *_researchManager;
    PlayerUpgradeManager    *_upgradeManager;
    PlayerResourceMap       *_resourceMap;
    
    MAXClanConfig* _clanConfig;
    CCPoint _landingPosition;
    
    GameMatchPlayerInfo _playerInfo;
    GameFog* fogs[FOG_TYPE_MAX];
    
    USimpleContainer<GameUnit*> _units;
    PlayerBase* _base;
    
    GameMatchPlayer(GameMatchPlayerInfo playerInfo, GameMatch *match);
    ~GameMatchPlayer();

    void SetPalette(double time);
    void LandingTo(const CCPoint &landingPosition);
    void BeginTurn();

    bool CanSeeUnit(GameUnit* unit);
    GameUnit* CreateUnit (int posx, int posy, string type, unsigned int ID);
    
#pragma mark - GameFogDelegate

    virtual float UnitScanRadiusForFog(const GameUnit *unit, const GameFog *fog) const;
    virtual void CellDidUpdate(const int cellX, const int cellY, const GameFog *fog, bool visibleFlag) const;
    
#pragma mark - GameUnitDelegate 
        
    virtual void GameUnitWillLeaveCell(GameUnit *unit);
    virtual void GameUnitDidEnterCell(GameUnit *unit);
        
    virtual void GameUnitDidDestroy(GameUnit *unit);
        
    virtual void GameUnitDidPlaceOnMap(GameUnit *unit);
    virtual void GameUnitDidRemoveFromMap(GameUnit *unit);
    
    virtual void GameUnitDidUndetected(GameUnit *unit);
    virtual void GameUnitDidDetected(GameUnit *unit);
};

#endif /* defined(__MAX__Game_MatchPlayer__) */
