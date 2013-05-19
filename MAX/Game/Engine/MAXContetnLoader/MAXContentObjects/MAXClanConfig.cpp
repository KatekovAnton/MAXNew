//
//  MAXClanConfig.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXClanConfig.h"
#include "StringUtils.h"
#include "MAXConfigManager.h"

using namespace std;

MAXClanConfig::MAXClanConfig(string config, int number):
_name(""),
_number(number)
{
    vector<string> clan = splitString(config, '\n');
    for (int i = 0; i < clan.size(); i++) {
        string clani = clan[i];
        if (clan[i].length() == 0)
            continue;
        vector<string> keyValue = splitString(clan[i], '=');
        
        if (keyValue.size() != 2)
            continue;
        SetConfigValue(keyValue[0], keyValue[1]);
    }
}

MAXClanConfig::~MAXClanConfig()
{
}

void MAXClanConfig::SetConfigValue(string key, string value)
{
    if (key == "Name")
    {
        _name = value;
        return;
    }
    
    if (key == "Text")
    {
        _engText = value;
        return;
    }
    
    if (key == "Text-Rus")
    {
        return;
    }
    
    if (key == "Flags")
    {
        _flags = atoi(value.c_str());
        return;
    }
    
    if (MAXConfigManager::SharedMAXConfigManager()->ContainsUnit(key))
    {
        string unitType = toLower(key);
        vector<string> params = splitString(value, ", ");
        if (params.size() > 0)
        {
            vector<MAXClanUnitParameters> unitMods;
            for (int i = 0; i < params.size(); i++)
            {
                vector<string> keyValue = splitString(params[i], ' ');
                if (keyValue.size() != 2) 
                    continue;
                
                MAXClanUnitParameters params;
                params._paramName = keyValue[1];
                params._modValue = atoi(keyValue[0].c_str());
                unitMods.push_back(params);
            }
            
            _modifications[unitType] = unitMods;
            return;
        }
        else
        {
            vector<MAXClanUnitParameters> unitMods;
            
            vector<string> keyValue = splitString(value, ' ');
            MAXClanUnitParameters params;
            params._paramName = keyValue[1];
            params._modValue = atoi(keyValue[0].c_str());
            
            unitMods.push_back(params);
            
            
            return;
        }
    }
    
}


