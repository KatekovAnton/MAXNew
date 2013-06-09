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
#include "GameUnitDelegate.h"
#include "GameMatchPlayerDataDelegate.h"

class Texture;
class GameUnit;
class GameMatch;
class GameUnitBaseParameters;
class GameMatchPlayerData;
class MatchMapAgregator;
class Pathfinder;

class GameMatchPlayer : public GameUnitDelegate, public GameMatchPlayerDataDelegate {
    
    vector<Texture*> _palettes;
    Texture* _palette;
    
public:
    
    MatchMapAgregator*          _agregator;
    Pathfinder*                 _pathfinder;
    GameMatch *_match_w;
    
    GameMatchPlayerData *_playerData;
    
    bool GetIsCurrentPlayer() const;
    int GetPlayerId() const;
    Texture** GetPalettePointer() {return &_palette;};
    
    USimpleContainer<GameUnit*> _units;
    
    GameMatchPlayer(GameMatchPlayerInfo playerInfo, GameMatch *match);
    ~GameMatchPlayer();

    void SetPalette(double time);
    void LandingTo(const CCPoint &landingPosition);
    void BeginTurn();
    void EndTurn();

    bool CanSeeUnit(GameUnit* unit);
    GameUnit* CreateUnit (int posx, int posy, string type, unsigned int ID);
    
#pragma mark - GameMatchPlayerDataDelegate
    
    virtual EXTENDED_GROUND_TYPE GroudTypeAtPoint(const int x, const int y);
    virtual void CellDidUpdate(const int x, const int y, const FOG_TYPE type, const bool visibleFlag);
    
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
