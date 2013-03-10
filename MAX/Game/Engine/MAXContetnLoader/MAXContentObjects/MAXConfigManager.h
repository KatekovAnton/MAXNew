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
    map<string, MAXObjectConfig*> _clanConfigs;
    
    void LoadUnitSegment(string source);
    
public:
    
    static MAXConfigManager* SharedMAXConfigManager();
    
    void LoadConfigsFromFile(string file);
    void LoadConfigsFromString(string file);
    MAXObjectConfig* GetConfig(string type);
    
};

#endif /* defined(__MAX__MAXConfigManager__) */
