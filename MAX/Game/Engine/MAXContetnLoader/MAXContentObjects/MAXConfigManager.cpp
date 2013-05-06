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
        _unitConfigs.insert(pair<string, MAXObjectConfig*>(toLower(type), config));
    }
}

void MAXConfigManager::LoadConfigsFromFile(const string& file)
{
    _unitConfigs.clear();
    BinaryReader* reader = new BinaryReader(file);
    string strContent = reader->ReadFullAsString();
	removeBadCharacters(strContent);
    delete reader;
    
    LoadConfigsFromString(strContent);
    
}

void MAXConfigManager::LoadConfigsFromString(const string& strContent)
{
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
    _clanConfigs.clear();
    BinaryReader* reader = new BinaryReader(file);
    string strContent = reader->ReadFullAsString();
	removeBadCharacters(strContent);
    delete reader;
    
    LoadClanConfigsFromString(strContent);
}

void MAXConfigManager::LoadClanConfigsFromString(const string& strContent)
{
    vector<string> components1 = splitString(strContent, "[Clan ");
    for (int i = 0; i < components1.size(); i++)
    {
        string clanComponent = components1[i];
        vector<string> clan = splitString(clanComponent, "]\n");
        
        clanComponent = clan[1];
        MAXClanConfig* clanConfig = new MAXClanConfig(clanComponent, i);
        
    }
}

MAXObjectConfig* MAXConfigManager::GetConfig(const string& type)
{
    MAXObjectConfig* result = NULL;
    string realType = toLower(type);
    if (_unitConfigs.count(realType) == 1) {
        result = _unitConfigs[realType];
        return result;
    }
    return NULL;
}

bool MAXConfigManager::ContainsUnit(const string& type) const
{
    string realType = toLower(type);
    return _unitConfigs.count(realType) == 1;
}


