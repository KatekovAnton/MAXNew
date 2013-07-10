//
//  MAXConfigManager.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXConfigManager.h"
#include "MAXObjectConfig.h"
#include "MAXClanConfig.h"
#include "BinaryReader.h"
#include "StringUtils.h"

MAXConfigManager* _sharedMAXConfigManager = NULL;

MAXConfigManager* MAXConfigManager::SharedMAXConfigManager()
{
    if(!_sharedMAXConfigManager)
        _sharedMAXConfigManager = new MAXConfigManager();
    return _sharedMAXConfigManager;
}

void MAXConfigManager::LoadUnitSegment(const string& source)
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

        MAXObjectConfig* config = new MAXObjectConfig(balanceConfig, resourceConfig);
        config->_type = type;
        _unitConfigs.insert(pair<string, MAXObjectConfig*>(toLower(type), config));
        _allUnits.push_back(toLower(type));
    }
}

void MAXConfigManager::LoadConfigsFromFile(const string& file)
{
    BinaryReader* reader = new BinaryReader(file);
    string strContent = reader->ReadFullAsString();
    delete reader;
    
    LoadConfigsFromString(strContent);   
}

void MAXConfigManager::LoadConfigsFromString(string& strContent)
{
	removeBadCharacters(strContent);
    for (int i = 0; i < _allUnits.size(); i++) 
	{
        MAXObjectConfig* config = _unitConfigs[_allUnits[i]];
        delete config;
    }
    _unitConfigs.clear();
    _allUnits.clear();

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

void MAXConfigManager::LoadClanConfigsFromFile(const string& file)
{
    BinaryReader* reader = new BinaryReader(file);
    string strContent = reader->ReadFullAsString();
	removeBadCharacters(strContent);
    delete reader;
    
    LoadClanConfigsFromString(strContent);
}

void MAXConfigManager::LoadClanConfigsFromString(string& strContent)
{
	removeBadCharacters(strContent);
    for (int i = 0; i < _allClans.size(); i++) {
        MAXClanConfig* config = _clanConfigs[_allClans[i]];
        delete config;
    }
    _clanConfigs.clear();
    _allClans.clear();
    
    vector<string> components1 = splitString(strContent, "[Clan ");
    for (int i = 0; i < components1.size(); i++)
    {
        string clanComponent = components1[i];
        vector<string> clan = splitString(clanComponent, "]\n");
        int number = atoi(clan[0].c_str());
		if (_clanConfigs.count(number) != 0)
			continue;
		clanComponent = clan[1];
        MAXClanConfig* clanConfig = new MAXClanConfig(clanComponent, number);
        _clanConfigs[number] = clanConfig;
        _allClans.push_back(number);
    }
}

MAXObjectConfig* MAXConfigManager::GetUnitConfig(const string& type)
{
    string realType = toLower(type);
    if (_unitConfigs.count(realType) == 1) 
        return _unitConfigs[realType];
    
    return NULL;
}

MAXClanConfig* MAXConfigManager::GetClanConfig(const int type)
{
    if (_clanConfigs.count(type) == 1) 
        return _clanConfigs[type];
    
    return NULL;
}

bool MAXConfigManager::ContainsUnit(const string& type) const
{
    string realType = toLower(type);
    return _unitConfigs.count(realType) == 1;
}

vector<string> MAXConfigManager::ConstructableUnitsForConstructorType(int type)
{
    vector<string> result;
    if (type == 0)
        return result;
    
    vector<string> allUnits =GetAllUnits();
    for (int i = 0; i < allUnits.size(); i++)
    {
        string unittype = allUnits[i];
        MAXObjectConfig* params = _unitConfigs[unittype];
        if (params->_bSelfCreatedType == type)
            result.push_back(unittype);
    }
    
    return result;
}


