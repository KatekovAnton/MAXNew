//
//  GameUnitBaseParameters.h
//  MAX
//
//  Created by Anton Katekov on 15.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameUnitBaseParameters__
#define __MAX__GameUnitBaseParameters__

#include <iostream>

class MAXObjectConfig;
class MAXClanConfig;

class GameUnitBaseParameters {
    
    int _pSpeed;
    int _pHealth;
    int _pArmor;
    int _pAttack;
    int _pShots;
    int _pFuel;
    int _pRange;
    int _pScan;
    int _pAmmo;
    int _pCost;
    
public:
    
    MAXObjectConfig *_configObject;
    MAXClanConfig   *_configClan;
    
    GameUnitBaseParameters(MAXObjectConfig *unitConfig, MAXClanConfig *clanConfig);
    GameUnitBaseParameters(MAXObjectConfig *unitConfig);
    ~GameUnitBaseParameters();
    
#pragma mark - Getters
    
    int GetPSpeed() const {return _pSpeed;};
    int GetPHealth() const {return _pHealth;};
    int GetPArmor() const {return _pArmor;};
    int GetPAttack() const {return _pAttack;};
    int GetPShots() const {return _pShots;};
    int GetPFuel() const {return _pFuel;};
    int GetPRange() const {return _pRange;};
    int GetPScan() const {return _pScan;};
    int GetPAmmo() const {return _pAmmo;};
    int GetPCost() const {return _pCost;};
    
    ///const params
    int GetBSize() const;
    int GetBMoveType() const;
    int GetBLevel() const;
    int GetBNumber() const;
    int GetBSelfCreatedType() const;
    int GetBSelfCreatorType() const;
    
    bool GetIsBuilding() const;
    bool GetIsNeedUndercover() const;
    bool GetIsAllwaysOn() const;
    bool GetIsCantSelect() const;
    bool GetIsAbsCantSelect() const;
    bool GetIsStealth() const;
    bool GetIsAntiStealth() const;
    bool GetIsMine() const;
    bool GetIsInfantry() const;
    bool GetIsConnector() const;
	bool GetIsPlatform() const;
    bool GetCanBuildHere() const;
    bool GetIsBridge() const;
    bool GetIsRoad() const;
    bool GetIsRetranslator() const;
    bool GetIsBuldozer() const;
    bool GetIsRepair() const;
    bool GetIsInfiltrator() const;
    bool GetIsBombMine() const;
    bool GetIsBombMinelayer() const;
    bool GetIsAutorepair() const;
    bool GetIsGivePoints() const;
    bool GetIsReloader() const;
    bool GetIsResearch() const;
    bool GetIsLanding() const;
    bool GetIsSurvivor() const;
    bool GetIsSpacePort() const;
    bool GetIsUpgrades() const;
    bool GetIsCanBuy() const;
    bool GetIsAnimated() const;
    bool GetIsUnderwater() const;
    bool GetIsSeeUnderwater() const;
    bool GetIsSeeMines() const;
    
    int GetPZone() const;
    int GetPMoveAndShot() const;
    int GetPFireType() const;
    int GetPBulletType() const;
    int GetPSeparateCanon() const;
    int GetPMatPerTurn() const;
    
    
    
    bool GetIsPlane() const;
    bool GetIsShip() const;
    bool GetIsStealthable() const;
    bool GetHasHead() const;
    bool GetIsAnimatedHead() const;
    bool GetIsAmphibious() const;
    
};

#endif /* defined(__MAX__GameUnitBaseParameters__) */
