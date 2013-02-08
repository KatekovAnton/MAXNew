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
    char data[5];
    data[0] = 0xD4;
    data[1] = 0xE0;
    data[2] = 0xE9;
    data[3] = 0xEB;
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
}

MAXUnitConfig::MAXUnitConfig(string balanceConfigName, string resourceConfigName)
:_bodyName(""), _imageName(""), _shadowName(""), _instoreName(""), _type("");
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
    //    cout<<lines[i] <<'\n';
    }
    
    r = new BinaryReader(balanceConfigName);
    string balanceConfig = r->ReadFullAsString();
    delete r;
    //Тип=Tank
    //Имя=Танк
    //Имя-Eng=Tank
    //
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
    //
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
    //
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
    //
    //Resources=0   0   0   0   0
    ////        мат топ зол эн  люд
    //
    //Store=0  0  0  0
    ////    Ю  В  М  П
    //
    //Needs  =0   0   0   0   0   0
    //Returns=0   0   0   0   0   0
    ////      мат топ зол гзл энр люд
    //
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