//
//  MAXObjectConfig.h
//  MAX
//
//  Created by Anton Katekov on 07.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXObjectConfig__
#define __MAX__MAXObjectConfig__

#include <iostream>
#include "miniPrefix.h"

typedef enum
{
    OBJECT_LEVEL_UNDERWATER   = 0,
    OBJECT_LEVEL_ONGROUND     = 1,
    OBJECT_LEVEL_OVERGROUND   = 2,
    OBJECT_LEVEL_BUILDING     = 3,
    OBJECT_LEVEL_UNITS        = 4,
    OBJECT_LEVEL_OVERUNITS    = 5,
    OBJECT_LEVEL_AIR          = 6,
    OBJECT_LEVEL_OVERAIR      = 7
} OBJECT_LEVEL;

typedef enum
{
    UNIT_MOVETYPE_GROUND = 0,
    UNIT_MOVETYPE_GROUNDCOST = 1,
    UNIT_MOVETYPE_AMHIB = 2,
    UNIT_MOVETYPE_SEACOST = 3,
    UNIT_MOVETYPE_SEA = 4,
    UNIT_MOVETYPE_AIR = 5
} UNIT_MOVETYPE;

class MAXObjectConfig {
    
    void SetResurceConfigValue(string key, string value);
    void SetBalanceConfigValue(string key, string value);
public:
    //Тип=Tank
    string _type;
    
    //КадрыБазы=0,7,2
    //КадрыПушки=8,15,2
    //КадрыПушкиСтрел=16,23,2
    bool _isAnimBase;
    bool _isAnimHead;
    bool _isAbleToFire;
    
    //ФайлB=TANK
    string _bodyName;
    
    //ФайлF=P_TANK
    string _imageName;
    
    //ФайлV=Tank03.flc
    //ФайлI=I_TANK,1
    
    //ФайлA=A_TANK
    string _instoreName;
    
    //ФайлS=S_TANK
    string _shadowName;
    
    ////Sound//
    //Взрывзвук=blast-15.wav
    //Выстзвук=shoot-0.wav
    //Запзвук=on.wav
    //Рабзвук=hld.wav
    //Стрзвук=-1
    //Стопзвук=off.wav
    //Двигзвук=run.wav
    //Степзвук=off.wav
    
    
    string _name;
    //Basics=1   0   4   52  4   0
    ////     Р   П   У   Н   С   С
    ////     а   р   р   о   т   т
    ////     з   о   о   м   р   р
    ////     м   х   в   е   о   о
    ////     е   о   е   р   и   и
    ////     р   д   н       м   т
    ////         и   ь       ы   е
    ////         м           й   л
    ////         о               ь
    ////         с
    ////         т
    ////         ь
    int _bSize;
    int _bMoveType;
    int _bLevel;
    int _bNumber;
    int _bSelfCreatedType;
    int _bSelfCreatorType;
    
    //Features=00000000000000000000000001010000
    ////       ЗПВНАСВШЧКНМДПБРДММСОПИПГЗОПАПВВ
    ////       досебтиаеоаооруеиииачесоевбоноии
    ////       адевседхлнссролмвннмкрссоенкиддд
    ////       нкгынлитонттовьоеаооиелалзоумвии
    ////       илдбеставер годнр урдзедодвпаотт
    ////       еавив с еко адотс кеаадоголацдпм
    ////        дкры т кте  нзеа лмёруч пееинои
    ////        клаб е  ом  иерн аотяен онмяыдн
    ////        а еи л  ры  кр т дн жты риы йвы
    ////          мр с   й       чт а й тяй  о
    ////           а             и  е        д
    ////           е             к  т        о
    ////           м                         й
    bool _isBuilding;
    bool _isNeedPodkladka;
    bool _isAllwaysOn;
    bool _isCantSelect;
    bool _isAbsCantSelect;
    bool _isStealth;
    bool _isAntiStealth;
    bool _isMine;
    bool _isInfantry;
    bool _isConnector;
    bool _isCantCreate;
    bool _isBridge;
    bool _isRoad;
    bool _isRetranslator;
    bool _isBuldozer;
    bool _isRepair;
    bool _isInfiltrator;
    bool _isBombMine;
    bool _isBombMinelayer;
    bool _isAutorepair;
    bool _isGivePoints;
    bool _isReloader;
    bool _isResearch;
    bool _isLanding;
    bool _isSurvivor;
    bool _isSpacePort;
    bool _isUpgrades;
    bool _isCanBuy;
    bool _isAnimated;
    bool _isUnderwater;
    bool _isSeeUnderwater;
    bool _isSeeMines;
    
    //Parameters=6   24  10  16  2   42  4   4   14  12  0   0   1   1   1   0
    ////         С   П   Б   А   В   Т   Д   В   П   Ц   З   Х   Т   Т   О   М
    ////         к   р   р   т   ы   о   и   и   а   е   о   о   и   и   т   а
    ////         о   о   о   а   с   п   а   д   т   н   н   д   п   п   д   т
    ////         р   ч   н   к   т   л   п   м   р   а   а   о   о   с   .
    ////         о   н   я   а   р   и   а   о   о           с   г   т   п   в
    ////         с   о           е   в   з   с   н           т   н   р   у
    ////         т   с           л   о   о   т   ы           р   я   е   ш   х
    ////         ь   т           ы       н   ь               е       л   к   о
    ////             ь                                       л           а   д
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
    int _pZone;//explosive range
    int _pMoveAndShot;
    int _pFireType;
    int _pBulletType;
    int _pSeparateCanon;
    int _pMatPerTurn;
    
    //Resources=0   0   0   0   0
    ////        мат топ зол эн  люд
    int _rMaterial;
    int _rFuel;
    int _rGold;
    int _rEnergy;
    int _rPeoples;
    
    
    //Store=0  0  0  0
    ////    Ю  В  М  П
    int _sGround;
    int _sAir;
    int _sSea;
    int _sInfantry;
    
    //Needs  =0   0   0   0   0   0
    //Returns=0   0   0   0   0   0
    ////      мат топ зол гзл энр люд
    int _nMaterial;
    int _nFuel;
    int _nGold;
    int _nEnergy;
    int _nPeoples;
    
    int _retMaterial;
    int _retFuel;
    int _retGold;
    int _retEnergy;
    int _retPeoples;
    
    
    bool _isPlane;
    bool _isShip;
    GLKVector2 _shadowOffset;
    int _level;
    bool _hasHead;
    bool _isAnimatedHead;
    bool _isMultifire;
    bool _isAmphibious;
    
    MAXObjectConfig();
    MAXObjectConfig(string balanceConfigName, string resourceConfigName);
    
    ~MAXObjectConfig();
    bool IsValid() const;
    
    bool _haveShadow;
    
};

#endif /* defined(__MAX__MAXObjectConfig__) */
