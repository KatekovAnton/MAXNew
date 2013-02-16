//
//  MAXConfigManager.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXConfigManager.h"
#include "MAXUnitConfig.h"
#include "BinaryReader.h"
#include "StringUtils.h"

MAXConfigManager* _sharedMAXConfigManager = NULL;

MAXConfigManager* MAXConfigManager::SharedMAXConfigManager()
{
    if(!_sharedMAXConfigManager)
        _sharedMAXConfigManager = new MAXConfigManager();
    return _sharedMAXConfigManager;
}

void MAXConfigManager::LoadUnitSegment(string source)
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
        _unitConfigs.insert(pair<string, MAXUnitConfig*>(type, config));
    }
}

void MAXConfigManager::LoadConfigsFromFile(string file)
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

MAXUnitConfig* MAXConfigManager::GetConfig(string type)
{
    MAXUnitConfig* result = NULL;
    if (_unitConfigs.count(type) == 1) {
        result = _unitConfigs[type];
        return result;
    }
    return NULL;
}