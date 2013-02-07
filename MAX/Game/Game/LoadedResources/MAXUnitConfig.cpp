//
//  MAXUnitConfig.cpp
//  MAX
//
//  Created by Anton Katekov on 07.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXUnitConfig.h"
#include "BinaryReader.h"

//Тип=Tank
//
////Graphics//
//КадрыБазы=0,7,2
//КадрыПушки=8,15,2
//КадрыПушкиСтрел=16,23,2
//ФайлB=TANK
//ФайлF=P_TANK
//ФайлV=Tank03.flc
//ФайлI=I_TANK,1
//ФайлA=A_TANK
//ФайлS=S_TANK
//
////Sound//
//Взрывзвук=blast-15.wav
//Выстзвук=shoot-0.wav
//Запзвук=on.wav
//Рабзвук=hld.wav
//Стрзвук=-1
//Стопзвук=off.wav
//Двигзвук=run.wav
//Степзвук=off.wav

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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> splitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

MAXUnitConfigManager* _sharedMAXUnitConfigManager = NULL;

MAXUnitConfig::MAXUnitConfig(string balanceConfigName, string resourceConfigName)
{
    BinaryReader *r = new BinaryReader(resourceConfigName);
    string resourceConfig = r->ReadFullAsString();
    delete r;
    
    r = new BinaryReader(balanceConfigName);
    string balanceConfig = r->ReadFullAsString();
    delete r;
}

MAXUnitConfigManager* MAXUnitConfigManager::SharedMAXUnitConfigManager()
{
    if(!_sharedMAXUnitConfigManager)
        _sharedMAXUnitConfigManager = new MAXUnitConfigManager();
    return _sharedMAXUnitConfigManager;
}

void MAXUnitConfigManager::LoadConfigsFromFile(string file)
{
    BinaryReader* reader = new BinaryReader(file);
    string strContent = reader->ReadFullAsString();
    delete reader;
    
    
    vector<string> components = splitString(strContent, '\n');
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

MAXUnitConfig* MAXUnitConfigManager::GetConfig(string type)
{
    MAXUnitConfig* result = NULL;
    if (_configs.count(type) == 1) {
        result = _configs[type];
        return result;
    }
    return NULL;
}