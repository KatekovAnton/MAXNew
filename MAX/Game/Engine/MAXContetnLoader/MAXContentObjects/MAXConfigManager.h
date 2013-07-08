//
//  MAXConfigManager.h
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXConfigManager__
#define __MAX__MAXConfigManager__

#include <iostream>
#include "miniPrefix.h"

class MAXObjectConfig;
class MAXClanConfig;

class MAXConfigManager {
    
    map<string, MAXObjectConfig*> _unitConfigs;
    vector<string> _allUnits;
    map<int, MAXClanConfig*> _clanConfigs;
    vector<int> _allClans;
    
    void LoadUnitSegment(const string&  source);
    
public:
    
    static MAXConfigManager* SharedMAXConfigManager();
    
    void LoadConfigsFromFile(const string& file);
    void LoadConfigsFromString(string& strContent);
    
    void LoadClanConfigsFromFile(const string& file);
    void LoadClanConfigsFromString(string& strContent);
    
    vector<string> GetAllUnits() const { return _allUnits; };
    MAXObjectConfig* GetUnitConfig(const string& type);
    MAXClanConfig* GetClanConfig(const int type);
    
    bool ContainsUnit(const string& type) const;
    
    vector<string> ConstructableUnitsForConstructorType(int type);
};

#endif /* defined(__MAX__MAXConfigManager__) */
