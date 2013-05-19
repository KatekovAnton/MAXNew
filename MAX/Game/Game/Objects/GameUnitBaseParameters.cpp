//
//  GameUnitBaseParameters.cpp
//  MAX
//
//  Created by Anton Katekov on 15.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GameUnitBaseParameters.h"
#include "MAXObjectConfig.h"
#include "MAXClanConfig.h"

GameUnitBaseParameters::GameUnitBaseParameters(MAXObjectConfig *unitConfig, MAXClanConfig *clanConfig)
:_configObject(unitConfig), _configClan(clanConfig),
_pSpeed(unitConfig->_pSpeed),
_pHealth(unitConfig->_pHealth),
_pArmor(unitConfig->_pArmor),
_pAttack(unitConfig->_pAttack),
_pShots(unitConfig->_pShots),
_pFuel(unitConfig->_pFuel),
_pRange(unitConfig->_pRange),
_pScan(unitConfig->_pScan),
_pAmmo(unitConfig->_pAmmo),
_pCost(unitConfig->_pCost)
{
    vector<MAXClanUnitParameters> modifications = clanConfig->GetModificationsForType(unitConfig->_type);
    if (modifications.size()>0)
    {
        //implement this
        int a = 0;
        a++;
    }
}

GameUnitBaseParameters::GameUnitBaseParameters(MAXObjectConfig *unitConfig)
:_configObject(unitConfig),
_pSpeed(unitConfig->_pSpeed),
_pHealth(unitConfig->_pHealth),
_pArmor(unitConfig->_pArmor),
_pAttack(unitConfig->_pAttack),
_pShots(unitConfig->_pShots),
_pFuel(unitConfig->_pFuel),
_pRange(unitConfig->_pRange),
_pScan(unitConfig->_pScan),
_pAmmo(unitConfig->_pAmmo),
_pCost(unitConfig->_pCost)
{

}

GameUnitBaseParameters::~GameUnitBaseParameters()
{}

#pragma mark - Getters

int GameUnitBaseParameters::GetBSize() const {  return _configObject->_bSize; }
int GameUnitBaseParameters::GetBMoveType() const {  return _configObject->_bMoveType; }
int GameUnitBaseParameters::GetBLevel() const {  return _configObject->_bLevel; }
int GameUnitBaseParameters::GetBNumber() const { return _configObject->_bNumber; }
int GameUnitBaseParameters::GetBSelfCreatedType() const { return _configObject->_bSelfCreatedType; }
int GameUnitBaseParameters::GetBSelfCreatorType() const { return _configObject->_bSelfCreatorType; }

bool GameUnitBaseParameters::GetIsBuilding() const { return _configObject->_isBuilding; }
bool GameUnitBaseParameters::GetIsNeedUndercover() const { return _configObject->_isNeedUndercover; }
bool GameUnitBaseParameters::GetIsAllwaysOn() const { return _configObject->_isAllwaysOn; }
bool GameUnitBaseParameters::GetIsCantSelect() const { return _configObject->_isCantSelect; }
bool GameUnitBaseParameters::GetIsAbsCantSelect() const { return _configObject->_isAbsCantSelect; }
bool GameUnitBaseParameters::GetIsStealth() const { return _configObject->_isStealth; }
bool GameUnitBaseParameters::GetIsAntiStealth() const { return _configObject->_isAntiStealth; }
bool GameUnitBaseParameters::GetIsMine() const { return _configObject->_isMine; }
bool GameUnitBaseParameters::GetIsInfantry() const { return _configObject->_isInfantry; }
bool GameUnitBaseParameters::GetIsConnector() const { return _configObject->_isConnector; }
bool GameUnitBaseParameters::GetIsPlatform() const { return _configObject->_isPlatform; }
bool GameUnitBaseParameters::GetCanBuildHere() const { return _configObject->_canBuildHere; }
bool GameUnitBaseParameters::GetIsBridge() const { return _configObject->_isBridge; }
bool GameUnitBaseParameters::GetIsRoad() const { return _configObject->_isRoad; }
bool GameUnitBaseParameters::GetIsRetranslator() const { return _configObject->_isRetranslator; }
bool GameUnitBaseParameters::GetIsBuldozer() const { return _configObject->_isBuldozer; }
bool GameUnitBaseParameters::GetIsRepair() const { return _configObject->_isRepair; }
bool GameUnitBaseParameters::GetIsInfiltrator() const { return _configObject->_isInfiltrator; }
bool GameUnitBaseParameters::GetIsBombMine() const { return _configObject->_isBombMine; }
bool GameUnitBaseParameters::GetIsBombMinelayer() const { return _configObject->_isBombMinelayer; }
bool GameUnitBaseParameters::GetIsAutorepair() const { return _configObject->_isAutorepair; }
bool GameUnitBaseParameters::GetIsGivePoints() const { return _configObject->_isGivePoints; }
bool GameUnitBaseParameters::GetIsReloader() const { return _configObject->_isReloader; }
bool GameUnitBaseParameters::GetIsResearch() const { return _configObject->_isResearch; }
bool GameUnitBaseParameters::GetIsLanding() const { return _configObject->_isLanding; }
bool GameUnitBaseParameters::GetIsSurvivor() const { return _configObject->_isSurvivor; }
bool GameUnitBaseParameters::GetIsSpacePort() const { return _configObject->_isSpacePort; }
bool GameUnitBaseParameters::GetIsUpgrades() const { return _configObject->_isUpgrades; }
bool GameUnitBaseParameters::GetIsCanBuy() const { return _configObject->_isCanBuy; }
bool GameUnitBaseParameters::GetIsAnimated() const { return _configObject->_isAnimated; }
bool GameUnitBaseParameters::GetIsUnderwater() const { return _configObject->_isUnderwater; }
bool GameUnitBaseParameters::GetIsSeeUnderwater() const { return _configObject->_isSeeUnderwater; }
bool GameUnitBaseParameters::GetIsSeeMines() const { return _configObject->_isSeeMines; }

int GameUnitBaseParameters::GetPZone() const { return _configObject->_pZone; }
int GameUnitBaseParameters::GetPMoveAndShot() const { return _configObject->_pMoveAndShot; }
int GameUnitBaseParameters::GetPFireType() const { return _configObject->_pFireType; }
int GameUnitBaseParameters::GetPBulletType() const { return _configObject->_pBulletType; }
int GameUnitBaseParameters::GetPSeparateCanon() const { return _configObject->_pSeparateCanon; }
int GameUnitBaseParameters::GetPMatPerTurn() const { return _configObject->_pMatPerTurn; }

bool GameUnitBaseParameters::GetIsPlane() const { return _configObject->_isPlane; }
bool GameUnitBaseParameters::GetIsShip() const { return _configObject->_isShip; }
bool GameUnitBaseParameters::GetIsStealthable() const { return _configObject->_isStealthable; }
bool GameUnitBaseParameters::GetHasHead() const { return _configObject->_hasHead; }
bool GameUnitBaseParameters::GetIsAnimatedHead() const { return _configObject->_isAnimatedHead; }
bool GameUnitBaseParameters::GetIsAmphibious() const { return _configObject->_isAmphibious; }

