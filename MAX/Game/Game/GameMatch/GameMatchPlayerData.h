//
//  GameMatchPlayerData.h
//  MAX
//
//  Created by  Developer on 07.06.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameMatchPlayerData__
#define __MAX__GameMatchPlayerData__

#include <iostream>
#include "miniPrefix.h"
#include "PlayerResearchManager.h"
#include "GameFogDelegate.h"

class PlayerResearchManager;
class PlayerUpgradeManager;
class PlayerResourceMap;

class MAXClanConfig;

class GameUnitBaseParameters;
class GameMatch;
class GameFog;

class PlayerBase;
class GameUnitData;
class GameMatchPlayerDataDelegate;

class GameMatchPlayerData : public GameFogDelegate {
    
public:
    
    GameMatchPlayerDataDelegate *_delegate_w;
    
    GameMatchPlayerInfo _playerInfo;
    
    map<string, GameUnitBaseParameters*> _unitConfigs;
    MAXClanConfig* _clanConfig;
    
    CCPoint _landingPosition;
    
    int researchCentersWorkingOnArea[kNrResearchAreas]; ///< counts the number of research centers that are currently working on each area
    PlayerResearchManager   *_researchManager;
    PlayerUpgradeManager    *_upgradeManager;
    PlayerResourceMap       *_resourceMap;
        
    GameFog* fogs[FOG_TYPE_MAX];
    
    PlayerBase* _base;
    
    CCPoint cameraPosition;
    float cameraZoom;
    
    GameMatchPlayerData(GameMatchPlayerInfo playerInfo, GameMatch *match);
    ~GameMatchPlayerData();
    
    bool CanSeeUnit(GameUnitData* unit);
    bool UnitShouldUpdateFog(const GameUnitData *unit, const GameFog *fog) const;
    bool UnitCoveredByFog(const GameUnitData *unit, const GameFog *fog) const;
    
    void UnitDidStartMove(GameUnitData *unit);
    void UnitDidEndMove(GameUnitData *unit);
    void UnitDidRemoveFromMap(GameUnitData *unit);
    void UnitDidPlaceToMap(GameUnitData *unit);
    
    
#pragma mark - GameFogDelegate
    
    virtual float UnitScanRadiusForFog(const GameUnitData *unit, const GameFog *fog) const;
    virtual void CellDidUpdate(const int cellX, const int cellY, const GameFog *fog, bool visibleFlag) const;
};

#endif /* defined(__MAX__GameMatchPlayerData__) */
