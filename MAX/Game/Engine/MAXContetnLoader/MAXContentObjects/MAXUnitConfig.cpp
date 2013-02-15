//
//  MAXUnitConfig.cpp
//  MAX
//
//  Created by Anton Katekov on 07.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXUnitConfig.h"
#include "BinaryReader.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    string::size_type pos = 0;
    while (true)
    {
        string::size_type startPos = s.find_first_of(delim, pos);
        if (startPos == std::string::npos)
        {
            if (pos != s.length()) 
                elems.push_back(s.substr(pos, s.length() - pos));
            
            break;
        }
        elems.push_back(s.substr(pos, startPos - pos));
        pos = startPos + 1;
    }
    
//    while(std::getline(ss, item, delim))
//    {
//        elems.push_back(item);
//    }
    return elems;
}

void split1(const string& str, const string& delimiters , vector<string>& tokens)
{
    string::size_type pos = 0;
    while (true)
    {
        string::size_type startPos = str.find(delimiters.c_str(), pos);
        if (startPos == std::string::npos) 
            break;
        startPos += delimiters.length();
        string::size_type endPos = str.find(delimiters.c_str(), startPos+2);
        if (endPos == std::string::npos) 
            break;
        
        
        tokens.push_back(str.substr(startPos, endPos - startPos));
        pos = endPos;
    }
}

std::vector<std::string> splitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<std::string> splitString(const std::string &s, string delim)
{
    std::vector<std::string> elems;
    split1(s, delim, elems);
    return elems;
}

std::vector<std::string> getElements(string value)
{
    size_t index = 0;
    while (true) {
        index = value.find("  ", 0);
        if (index == string::npos)
            break;
        value.replace(index, 2, " ");
    }
    vector<string> elements = splitString(value, ' ');
    return elements;
}
    
const string commentSign = "//";

vector<string> removeCommentsAndEmptyStrings(vector<string> lines)
{
    vector<string> result;
    
    for (int i = 0; i < lines.size(); i++)
    {
        string str = lines[i];
        if (str.length() == 0) {
            continue;
        }
        string::size_type startPos = str.find(commentSign.c_str(), 0);
        if (startPos != std::string::npos)
        {
            if (startPos == 0)
                continue;
            
            string result1 = str.substr(0, startPos);
            result.push_back(result1);
        }
        else
            result.push_back(str);
    }
    
    return result;
}

MAXUnitConfigManager* _sharedMAXUnitConfigManager = NULL;

void MAXUnitConfig::SetResurceConfigValue(string key, string value)
{
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
    
    //кадрыбазыаним
    static unsigned char dataBaseAnim[] = {0xCA, 0xE0, 0xE4, 0xF0, 0xFB, 0xC1, 0xE0, 0xE7, 0xFB, 0xC0, 0xED, 0xE8, 0xEC};
    res = memcmp(dataBaseAnim, key.c_str(), 13);
    if (res == 0)
    {
        _isAnimHead = true;
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
    
}

void MAXUnitConfig::SetBalanceConfigValue(string key, string value)
{
    unsigned char name[3] = {0xC8, 0xEC, 0xFF};
    if (memcmp(name, key.c_str(), 3) == 0 && false) {
        _name = value;
        return;
    }
    
    unsigned char nameEng[7] = {0xC8, 0xEC, 0xFF, 0x2D, 0x45, 0x6E, 0x67};
    if (memcmp(nameEng, key.c_str(), 3) == 0 && false) {
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
        _isNeedPodkladka    = value[1] == 0x31;
        _isAllwaysOn        = value[2] == 0x31;
        _isCantSelect       = value[3] == 0x31;
        _isAbsCantSelect    = value[4] == 0x31;
        _isStealth          = value[5] == 0x31;
        _isAntiStealth      = value[6] == 0x31;
        _isMine             = value[7] == 0x31;
        _isInfantry         = value[8] == 0x31;
        _isConnector        = value[9] == 0x31;
        _isCantCreate       = value[10] == 0x31;
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
    
    if (key == "Needs")
    {
        //Needs  =0   0   0   0   0   0
        vector<string> elements = getElements(value);
        _nMaterial          = atoi(elements[0].c_str());
        _nFuel              = atoi(elements[1].c_str());
        _nGold              = atoi(elements[2].c_str());
        _nEnergy            = atoi(elements[3].c_str());
        _nPeoples           = atoi(elements[4].c_str());
        
        return;
    }
    
    if (key == "Returns")
    {
        //Returns=0   0   0   0   0   0
        vector<string> elements = getElements(value);
        _retMaterial            = atoi(elements[0].c_str());
        _retFuel                = atoi(elements[1].c_str());
        _retGold                = atoi(elements[2].c_str());
        _retEnergy              = atoi(elements[3].c_str());
        _retPeoples             = atoi(elements[4].c_str());
        
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

MAXUnitConfig::MAXUnitConfig(string balanceConfigName, string resourceConfigName)
:_bodyName(""), _imageName(""), _shadowName(""), _instoreName(""), _type(""), _isAnimBase(false), _isAnimHead(false), _isAbleToFire(false)
{
    BinaryReader *r = new BinaryReader(resourceConfigName);
    string resourceConfig = r->ReadFullAsString();
    std::remove(resourceConfig.begin(), resourceConfig.end(), '\r');
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
    
    }

MAXUnitConfigManager* MAXUnitConfigManager::SharedMAXUnitConfigManager()
{
    if(!_sharedMAXUnitConfigManager)
        _sharedMAXUnitConfigManager = new MAXUnitConfigManager();
    return _sharedMAXUnitConfigManager;
}

void MAXUnitConfigManager::LoadUnitSegment(string source)
{
    vector<string> components = splitString(source, '\n');
    for (int i = 0; i < components.size(); i++)
    {
        string str = components[i];
        if (str.length()<1)
            continue;
        
        vector<string> typeData = splitString(str, '=');
        
        string type = typeData[0];
        vector<string> data = splitString(typeData[1], '#');
        string resourceConfig = data[0];
        string balanceConfig = data[1];
        
        MAXUnitConfig* config = new MAXUnitConfig(balanceConfig, resourceConfig);
        _configs.insert(pair<string, MAXUnitConfig*>(type, config));
    }
}

void MAXUnitConfigManager::LoadConfigsFromFile(string file)
{
    BinaryReader* reader = new BinaryReader(file);
    string strContent = reader->ReadFullAsString();
    delete reader;
    
    vector<string> components1 = splitString(strContent, "#segment=");
    for (int i = 0; i < components1.size(); i++)
    {
        string component = components1[i];
        int pos = component.find_first_of('\n');
        string segmentName = component.substr(0, pos);
        if(segmentName == "units")
        {
            string segmentSource = component.substr(pos+1, component.length() - pos - 1);
            LoadUnitSegment(segmentSource);
        }
    }
    
}

MAXUnitConfig* MAXUnitConfigManager::GetConfig(string type)
{
    MAXUnitConfig* result = NULL;
    if (_configs.count(type) == 1) {
        result = _configs[type];
        return result;
    }
    return NULL;
}

bool MAXUnitConfig::IsValid() const
{
    return _type.length()>0 &&
    _bodyName.length()>0    &&
    _instoreName.length()>0 &&
    _shadowName.length()>0  &&
    _imageName.length()>0;
}