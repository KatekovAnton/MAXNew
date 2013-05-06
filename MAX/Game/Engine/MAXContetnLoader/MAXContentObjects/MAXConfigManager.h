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
    map<string, MAXClanConfig*> _clanConfigs;
    
    void LoadUnitSegment(const string&  source);
    
public:
    
    static MAXConfigManager* SharedMAXConfigManager();
    
    void LoadConfigsFromFile(const string& file);
    void LoadConfigsFromString(const string& strContent);
    
    void LoadClanConfigsFromFile(const string& file);
    void LoadClanConfigsFromString(const string& strContent);
    
    MAXObjectConfig* GetConfig(const string& type);
    bool ContainsUnit(const string& type) const;
};

#endif /* defined(__MAX__MAXConfigManager__) */
