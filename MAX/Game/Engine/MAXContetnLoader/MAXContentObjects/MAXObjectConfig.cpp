//
//  MAXObjectConfig.cpp
//  MAX
//
//  Created by Anton Katekov on 07.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXObjectConfig.h"
#include "BinaryReader.h"
#include "StringUtils.h"

void MAXObjectConfig::SetResurceConfigValue(string key, string value)
{
    
    if (value.at(value.size()-1) == '\r')
        value.resize(value.length()-1);
    //Файл
    unsigned char data[5];
    data[0] = 0xD4;data[1] = 0xE0;data[2] = 0xE9;data[3] = 0xEB;
    
    data[4] = 'B';
    int res = memcmp(data, key.c_str(), 5);
    if (res == 0)
    {
        _bodyName = value;
        return;
    }
    
    data[4] = 'A';
    res = memcmp(data, key.c_str(), 5);
    if (res == 0)
    {
        _instoreName = value;
        return;
    }
    
    data[4] = 'S';
    res = memcmp(data, key.c_str(), 5);
    if (res == 0)
    {
        _shadowName = value;
        return;
    }
    
    data[4] = 'F';
    res = memcmp(data, key.c_str(), 5);
    if (res == 0)
    {
        _imageName = value;
        return;
    }
    
    data[4] = 'I';
    res = memcmp(data, key.c_str(), 5);
    if (res == 0)
    {
        vector<string> imageNumber;
        split(value, ',', imageNumber);
        _smallName = imageNumber[0];
        return;
    }
    
    //кадрыбазыаним
    static unsigned char dataBaseAnim[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xC1, 0xE0, 0xE7, 0xFB, 0xC0, 0xED, 0xE8, 0xEC};
    res = memcmp(dataBaseAnim, key.c_str(), 13);
    if (res == 0)
    {
        _isAnimHead = true;
        vector<string> result;
        split(value, ',' , result);
        bodyAnimFrame0 = atoi(result[0].c_str());
        bodyAnimFrame1 = atoi(result[1].c_str());
        bodyAnimFrame2 = atoi(result[2].c_str());
        return;
    }
    
    //кадрыпушкистрел
    static unsigned char dataHeadFire[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xCF, 0xF3, 0xF8, 0xEA, 0xE8, 0xD1, 0xF2, 0xF0, 0xE5, 0xEB};
    res = memcmp(dataHeadFire, key.c_str(), 13);
    if (res == 0)
    {
        _isAbleToFire = true;
        return;
    }
    
    //кадрыбазыстрел
    static unsigned char dataBaseFire[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xC1, 0xE0, 0xE7, 0xFB, 0xD1, 0xF2, 0xF0, 0xE5, 0xEB};
    res = memcmp(dataBaseFire, key.c_str(), 13);
    if (res == 0)
    {
        _isAbleToFire = true;
        return;
    }
    //КадрыРаботы
    static unsigned char dataHeadWork[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xD0, 0xE0, 0xE1, 0xEE, 0xF2, 0xFB};
    res = memcmp(dataHeadWork, key.c_str(), 11);
    if (res == 0)
    {
        _isActiveBody = true;
        vector<string> result;
        split(value, ',' , result);
        bodyActiveFrame0 = atoi(result[0].c_str());
        bodyActiveFrame1 = atoi(result[1].c_str());
        bodyActiveFrame2 = atoi(result[2].c_str());
       
        
        return;
    }
    //КадрыКон
    static unsigned char dataConWork[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xCA, 0xEE, 0xED};
    res = memcmp(dataConWork, key.c_str(), 8);
    if (res == 0)
    {
        //_isActiveBody = true;
        vector<string> result;
        split(value, ',' , result);
        connectorFrameStart = atoi(result[0].c_str());
        
        
        return;
    }
    //КадрыХода
    static unsigned char dataSteps[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xD5, 0xEE, 0xE4, 0xE0};
    res = memcmp(dataSteps, key.c_str(), 8);
    if (res == 0)
    {
        vector<string> result;
        split(value, ',' , result);
        stepFrameStart = atoi(result[0].c_str());
        stepFrameEnd = atoi(result[1].c_str());
        
        return;
    }
    
    
    //кадрыбазы
    static unsigned char dataBase[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xC1, 0xE0, 0xE7, 0xFB};
    res = memcmp(dataBase, key.c_str(), 9);
    if (res == 0)
    {
        vector<string> result;
        split(value, ',' , result);
        bodyIdleFrame0 = atoi(result[0].c_str());
        return;
    }
    
    //Взрывзвук=blast-15.wav
    static unsigned char dataSoundBlast[] = {0xC2, 0xE7, 0xF0, 0xFB, 0xE2, 0xE7, 0xE2, 0xF3, 0xEA};
    res = memcmp(dataSoundBlast, key.c_str(), 9);
    if (res == 0) {
        _soundBlastName = value;
        return;
    } 
    //Выстзвук=shoot-0.wav
    static unsigned char dataSoundShot[] = {0xC2, 0xFB, 0xF1, 0xF2, 0xE7, 0xE2, 0xF3, 0xEA};
    res = memcmp(dataSoundShot, key.c_str(), 8);
    if (res == 0) {
        _soundShotName = value;
        return;
    }
    //Запзвук=on.wav
    static unsigned char dataSoundStart[] = {0xC7, 0xE0, 0xEF, 0xE7, 0xE2, 0xF3, 0xEA};
    res = memcmp(dataSoundStart, key.c_str(), 7);
    if (res == 0) {
        _soundStartName = value;
        return;
    }
    //Рабзвук=hld.wav
    static unsigned char dataSoundWork[] = {0xD0, 0xE0, 0xE1, 0xE7, 0xE2, 0xF3, 0xEA};
    res = memcmp(dataSoundWork, key.c_str(), 7);
    if (res == 0) {
        _soundWorkName = value;
        return;
    }
    //Стрзвук=-1
    static unsigned char dataSoundBuild[] = {0xD1, 0xF2, 0xF0, 0xE7, 0xE2, 0xF3, 0xEA};
    res = memcmp(dataSoundBuild, key.c_str(), 7);
    if (res == 0) {
        _soundBuildName = value;
        return;
    }
    //Стопзвук=off.wav
    static unsigned char dataSoundStop[] = {0xD1, 0xF2, 0xEE, 0xEF, 0xE7, 0xE2, 0xF3, 0xEA};
    res = memcmp(dataSoundStop, key.c_str(), 8);
    if (res == 0) {
        _soundStopName = value;
        return;
    }
    
    //Двигстартзем=engin5.wav
    static unsigned char data_soundEngineStartName[] = {0xC4, 0xE2, 0xE8, 0xE3, 0xF1, 0xF2, 0xE0, 0xF0, 0xF2, 0xE7, 0xE5, 0xEC};
    res = memcmp(data_soundEngineStartName, key.c_str(), 12);
    if (res == 0) {
        _soundEngineStartName = value;
        return;
    }
    //Двигстартвод=engin6.wav
    static unsigned char data_soundEngineStartWaterName[] = {0xC4, 0xE2, 0xE8, 0xE3, 0xF1, 0xF2, 0xE0, 0xF0, 0xF2, 0xE2, 0xEE, 0xE4};
    res = memcmp(data_soundEngineStartWaterName, key.c_str(), 12);
    if (res == 0) {
        _soundEngineStartWaterName = value;
        return;
    }
    //Двигстопзем=engin7.wav
    static unsigned char data_soundEngineStopName[] = {0xC4, 0xE2, 0xE8, 0xE3, 0xF1, 0xF2, 0xEE, 0xEF, 0xE7, 0xE5, 0xEC};
    res = memcmp(data_soundEngineStopName, key.c_str(), 11);
    if (res == 0) {
        _soundEngineStopName = value;
        return;
    }
    //Двигстопвод=engin8.wav
    static unsigned char data_soundEngineStopWaterName[] = {0xC4, 0xE2, 0xE8, 0xE3, 0xF1, 0xF2, 0xEE, 0xEF, 0xE2, 0xEE, 0xE4};
    res = memcmp(data_soundEngineStopWaterName, key.c_str(), 11);
    if (res == 0) {
        _soundEngineStopWaterName = value;
        return;
    }
    //Двигзвукзем=run.wav
    static unsigned char data_soundEngineName[] = {0xC4, 0xE2, 0xE8, 0xE3, 0xE7, 0xE2, 0xF3, 0xEA, 0xE7, 0xE5, 0xEC};
    res = memcmp(data_soundEngineName, key.c_str(), 11);
    if (res == 0) {
        _soundEngineName = value;
        return;
    }
    //Двигзвуквод=engin2.wav
    static unsigned char data_soundEngineWaterName[] = {0xC4, 0xE2, 0xE8, 0xE3, 0xE7, 0xE2, 0xF3, 0xEA, 0xE2, 0xEE, 0xE4};
    res = memcmp(data_soundEngineWaterName, key.c_str(), 11);
    if (res == 0) {
        _soundEngineWaterName = value;
        return;
    }
    
}

void MAXObjectConfig::SetBalanceConfigValue(string key, string value)
{
    unsigned char name[3] = {0xC8, 0xEC, 0xFF};
    if (memcmp(name, key.c_str(), 3) == 0 && false) {
        _name = value;
        return;
    }
    
    unsigned char nameEng[7] = {0xC8, 0xEC, 0xFF, 0x2D, 0x45, 0x6E, 0x67};
    if (memcmp(nameEng, key.c_str(), 3) == 0) {
        _name = value;
        return;
    }
    
    if (key == "Basics")
    {
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
        
        vector<string> elements = getElements(value);
        _bSize               = atoi(elements[0].c_str());
        _bMoveType           = atoi(elements[1].c_str());
        _bLevel              = atoi(elements[2].c_str());
        _bNumber             = atoi(elements[3].c_str());
        _bSelfCreatedType    = atoi(elements[4].c_str());
        _bSelfCreatorType    = atoi(elements[5].c_str());
        return;
    }
    
    if (key == "Features")
    {
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
        _isBuilding         = value[0] == 0x31;
        _isNeedUndercover   = value[1] == 0x31;
        _isAllwaysOn        = value[2] == 0x31;
        _isCantSelect       = value[3] == 0x31;
        _isAbsCantSelect    = value[4] == 0x31;
        _isStealth          = value[5] == 0x31;
        _isAntiStealth      = value[6] == 0x31;
        _isMine             = value[7] == 0x31;
        _isInfantry         = value[8] == 0x31;
        _isConnector        = value[9] == 0x31;
        _canBuildHere       = value[10] == 0x31;
        _isBridge           = value[11] == 0x31;
        _isRoad             = value[12] == 0x31;
        _isRetranslator     = value[13] == 0x31;
        _isBuldozer         = value[14] == 0x31;
        _isRepair           = value[15] == 0x31;
        _isInfiltrator      = value[16] == 0x31;
        _isBombMine         = value[17] == 0x31;
        _isBombMinelayer    = value[18] == 0x31;
        _isAutorepair       = value[19] == 0x31;
        _isGivePoints       = value[20] == 0x31;
        _isReloader         = value[21] == 0x31;
        _isResearch         = value[22] == 0x31;
        _isLanding          = value[23] == 0x31;
        _isSurvivor         = value[24] == 0x31;
        _isSpacePort        = value[25] == 0x31;
        _isUpgrades         = value[26] == 0x31;
        _isCanBuy           = value[27] == 0x31;
        _isAnimated         = value[28] == 0x31;
        _isUnderwater       = value[29] == 0x31;
        _isSeeUnderwater    = value[30] == 0x31;
        _isSeeMines         = value[31] == 0x31;
        return;
    }
	_isPlatform = _canBuildHere && _isBuilding && !_isConnector;
    
    if (key == "Parameters")
    {
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
        
        vector<string> elements = getElements(value);
        
        _pSpeed             = atoi(elements[0].c_str());
        _pHealth            = atoi(elements[1].c_str());
        _pArmor             = atoi(elements[2].c_str());
        _pAttack            = atoi(elements[3].c_str());
        _pShots             = atoi(elements[4].c_str());
        _pFuel              = atoi(elements[5].c_str());
        _pRange             = atoi(elements[6].c_str());
        _pScan              = atoi(elements[7].c_str());
        _pAmmo              = atoi(elements[8].c_str());
        _pCost              = atoi(elements[9].c_str());
        _pZone              = atoi(elements[10].c_str());
        _pMoveAndShot       = atoi(elements[11].c_str());
        _pFireType          = atoi(elements[12].c_str());
        _pBulletType        = atoi(elements[13].c_str());
        _pSeparateCanon     = atoi(elements[14].c_str());
        _pMatPerTurn        = atoi(elements[15].c_str());
        
        _isAbleToFire = _pAttack != 0 && _pRange != 0 && _pAmmo != 0;
        return;
    }
    
    if (key == "Resources")
    {
        //Resources=0   0   0   0   0
        vector<string> elements = getElements(value);
        _rMaterial          = atoi(elements[0].c_str());
        _rFuel              = atoi(elements[1].c_str());
        _rGold              = atoi(elements[2].c_str());
        _rEnergy            = atoi(elements[3].c_str());
        _rPeoples           = atoi(elements[4].c_str());
        
        return;
    }
 
    if (key == "Store")
    {
        //Store=0  0  0  0
        vector<string> elements = getElements(value);
        _sGround            = atoi(elements[0].c_str());
        _sAir               = atoi(elements[1].c_str());
        _sSea               = atoi(elements[2].c_str());
        _sInfantry          = atoi(elements[3].c_str());
        
        return;
    }
    
    if (key == "Needs  ")
    {
        //Needs  =0   0   0   0   0   0
        vector<string> elements = getElements(value);
        _nMaterial          = atoi(elements[0].c_str());
        _nFuel              = atoi(elements[1].c_str());
        _nGold              = atoi(elements[2].c_str());
        _nMoney             = atoi(elements[3].c_str());
        _nEnergy            = atoi(elements[4].c_str());
        _nPeoples           = atoi(elements[5].c_str());
        return;
    }
    
    if (key == "Returns")
    {
        //Returns=0   0   0   0   0   0
        vector<string> elements = getElements(value);
        _retMaterial            = atoi(elements[0].c_str());
        _retFuel                = atoi(elements[1].c_str());
        _retGold                = atoi(elements[2].c_str());
        _retMoney               = atoi(elements[3].c_str());
        _retEnergy              = atoi(elements[4].c_str());
        _retPeoples             = atoi(elements[5].c_str());
        
        return;
    }
    
    
    //##begin_description(eng)
    //Heavily armored fighting vehicle.
    //Best used in the front line to prevent enemy units from reaching lightly-armored support units such as assault guns and rocket launchers.
    //##end_description
    //
    //##begin_description(rus)
    //Танк, это машина поля боя - тяжелобронированная медленная и мощная боевая техника, стреляющая обычными снарядами.
    //Обладая мощной броней танки всегда на острие атаки, вызывая огонь на себя, они взламывают любую оборону.
    //Эффективны против любой наземной техники.
    //##end_description

}

MAXObjectConfig::MAXObjectConfig()
:_bodyName(""),
_imageName(""),
_shadowName(""),
_instoreName(""),
_type(""),
_name(""),
_smallName(""),
_isAnimBase(false),
_isAnimHead(false),
_isAbleToFire(false),
_isBuilding(false),
_isActiveBody(false),
_pFuel(0),

_nMaterial(0),
_nFuel(0),
_nGold(0),
_nEnergy(0),
_nPeoples(0),

bodyAnimFrame0(0),
bodyAnimFrame1(0),
bodyAnimFrame2(0),
bodyActiveFrame0(0),
bodyActiveFrame1(0),
bodyActiveFrame2(0)
{
    
}

MAXObjectConfig::MAXObjectConfig(string balanceConfigName, string resourceConfigName)
:_bodyName(""),
_imageName(""),
_shadowName(""),
_instoreName(""),
_type(""),
_name(""),
_smallName(""),
_isAnimBase(false),
_isAnimHead(false),
_isAbleToFire(false),
_isBuilding(false),
_isActiveBody(false),
_pFuel(0),

_nMaterial(0),
_nFuel(0),
_nGold(0),
_nMoney(0),
_nEnergy(0),
_nPeoples(0),

_retMaterial(0),
_retFuel(0),
_retGold(0),
_retMoney(0),
_retEnergy(0),
_retPeoples(0),

bodyAnimFrame0(0),
bodyAnimFrame1(0),
bodyAnimFrame2(0),
bodyActiveFrame0(0),
bodyActiveFrame1(0),
bodyActiveFrame2(0),

_soundBlastName(""),
_soundShotName(""),
_soundStartName(""),
_soundWorkName(""),
_soundBuildName(""),
_soundStopName(""),
_soundEngineStartName(""),
_soundEngineStartWaterName(""),
_soundEngineStopName(""),
_soundEngineStopWaterName(""),
_soundEngineName(""),
_soundEngineWaterName("")
{

    BinaryReader *r = new BinaryReader(resourceConfigName);
    string resourceConfig = r->ReadFullAsString();
    delete r;
    vector<string> lines = splitString(resourceConfig, '\n');
    lines = removeCommentsAndEmptyStrings(lines);
    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> typeData = splitString(lines[i], '=');
        if (typeData.size() == 2)
            SetResurceConfigValue(typeData[0], typeData[1]);
    }
    
    r = new BinaryReader(balanceConfigName);
    string balanceConfig = r->ReadFullAsString();
    std::remove(balanceConfig.begin(), balanceConfig.end(), '\r');
    delete r;
    lines = splitString(balanceConfig, '\n');
    lines = removeCommentsAndEmptyStrings(lines);
    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> typeData = splitString(lines[i], '=');
        if (typeData.size() == 2)
            SetBalanceConfigValue(typeData[0], typeData[1]);
    }
    
    if (_isBombMine)
        _pFireType = 1;
    
    
    
    _isPlane = _bLevel == OBJECT_LEVEL_AIR;
    _hasHead = _pSeparateCanon == 1 || _isAnimHead;
    _isAnimatedHead = _isAnimHead;
//    if (_retMaterial > 0) {
//        int a = 0;
//        a++;
//    }
    if (_isSurvivor && (_bMoveType == UNIT_MOVETYPE_AMHIB))
    {
        _bMoveType = UNIT_MOVETYPE_SURVEYOR;
    }
    _isAmphibious = _bMoveType == UNIT_MOVETYPE_AMHIB || _bMoveType == UNIT_MOVETYPE_SURVEYOR;
    _isShip = ((_bMoveType == UNIT_MOVETYPE_SEA || _bMoveType == UNIT_MOVETYPE_SEACOAST)&& !_isBuilding) ||
    ((_bMoveType == UNIT_MOVETYPE_SEA || _bMoveType == UNIT_MOVETYPE_SEACOAST) && _isBuilding && _isBombMine);
    _isStealthable = _isStealth || _isUnderwater || _isBombMine;
    _haveShadow = _shadowName != "";
    
    _containProcessState = ((_isAllwaysOn != _isBuilding) || _bSelfCreatorType != 0 || _isBuldozer);
 
}

MAXObjectConfig::~MAXObjectConfig()
{}

bool MAXObjectConfig::IsValid() const
{
    return _type.length()>0 &&
    _bodyName.length()>0    &&
    _instoreName.length()>0 &&
    _shadowName.length()>0  &&
    _imageName.length()>0;
}

std::vector<UNIT_PARAMETER_TYPE> MAXObjectConfig::GetShortParameterList()
{
    std::vector<UNIT_PARAMETER_TYPE> result;
    
    if (_pHealth > 0)
        result.push_back(UNIT_PARAMETER_TYPE_HEALTH);
    if (_pFuel > 0)
        result.push_back(UNIT_PARAMETER_TYPE_GAS);
    if (_pAmmo > 0)
        result.push_back(UNIT_PARAMETER_TYPE_AMMO);
    if (_pSpeed > 0)
        result.push_back(UNIT_PARAMETER_TYPE_SPEED);
    if (_pShots > 0)
        result.push_back(UNIT_PARAMETER_TYPE_SHOTS);
    
    
    
    if (_isBuilding)
    {
        if (_retEnergy > 0)
        {
            result.push_back(UNIT_PARAMETER_TYPE_ENERGY);
            result.push_back(UNIT_PARAMETER_TYPE_ENERGY_BASE);
        }
        
        if (_retPeoples > 0)
        {
            result.push_back(UNIT_PARAMETER_TYPE_PEOPLE);
            result.push_back(UNIT_PARAMETER_TYPE_PEOPLE_BASE);
        }
        
        if (_nMaterial > 0 && _rMaterial == 0)
            result.push_back(UNIT_PARAMETER_TYPE_MATERIAL_BASE);
        if (_nFuel > 0)
            result.push_back(UNIT_PARAMETER_TYPE_FUEL_BASE);
        if (_nGold > 0)
            result.push_back(UNIT_PARAMETER_TYPE_GOLD_BASE);
        if (_nEnergy > 0 && _retEnergy == 0)
            result.push_back(UNIT_PARAMETER_TYPE_ENERGY_BASE);
        if (_nPeoples > 0)
            result.push_back(UNIT_PARAMETER_TYPE_PEOPLE_BASE);
    }

    
    if (_rMaterial > 0)
    {
        result.push_back(UNIT_PARAMETER_TYPE_MATERIAL);
        if (_isBuilding) 
            result.push_back(UNIT_PARAMETER_TYPE_MATERIAL_BASE);
    }
    
    if (_rFuel > 0)
        result.push_back(UNIT_PARAMETER_TYPE_FUEL);
    
    if (_rGold > 0)
        result.push_back(UNIT_PARAMETER_TYPE_GOLD);
    
        
    
    /*
    //caro
    UNIT_PARAMETER_TYPE_CARGO_UNITS,
    UNIT_PARAMETER_TYPE_CARGO_PLANES,
    UNIT_PARAMETER_TYPE_CARGO_SHIPS,
    */
    return result;
}

std::vector<UNIT_PARAMETER_TYPE> MAXObjectConfig::GetFullParameterList()
{
    std::vector<UNIT_PARAMETER_TYPE> result;
    /*
     //storage
     UNIT_PARAMETER_TYPE_FUEL,
     UNIT_PARAMETER_TYPE_GOLD,
     UNIT_PARAMETER_TYPE_MATERIAL,
     UNIT_PARAMETER_TYPE_PEOPLE,
     UNIT_PARAMETER_TYPE_ENERGY,
     
     //caro
     UNIT_PARAMETER_TYPE_CARGO_UNITS,
     UNIT_PARAMETER_TYPE_CARGO_PLANES,
     UNIT_PARAMETER_TYPE_CARGO_SHIPS,
     */
    if (_pHealth > 0)
        result.push_back(UNIT_PARAMETER_TYPE_HEALTH);
    if (_pArmor > 0)
        result.push_back(UNIT_PARAMETER_TYPE_ARMOR);
    if (_pAttack > 0)
        result.push_back(UNIT_PARAMETER_TYPE_ATTACK);
    if (_pFuel > 0)
        result.push_back(UNIT_PARAMETER_TYPE_GAS);
    if (_pRange > 0)
        result.push_back(UNIT_PARAMETER_TYPE_RANGE);
    if (_pScan > 0)
        result.push_back(UNIT_PARAMETER_TYPE_SCAN);
    if (_pAmmo > 0)
        result.push_back(UNIT_PARAMETER_TYPE_AMMO);
    if (_pSpeed > 0)
        result.push_back(UNIT_PARAMETER_TYPE_SPEED);
    if (_pShots > 0)
        result.push_back(UNIT_PARAMETER_TYPE_SHOTS);
    
    if (_rMaterial > 0)
        result.push_back(UNIT_PARAMETER_TYPE_MATERIAL);
    if (_rFuel > 0)
        result.push_back(UNIT_PARAMETER_TYPE_FUEL);
    if (_rGold > 0)
        result.push_back(UNIT_PARAMETER_TYPE_GOLD);
    
    return result;
}

