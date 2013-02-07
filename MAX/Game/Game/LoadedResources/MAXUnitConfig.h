//
//  MAXUnitConfig.h
//  MAX
//
//  Created by Anton Katekov on 07.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXUnitConfig__
#define __MAX__MAXUnitConfig__

#include <iostream>
#include "miniPrefix.h"

class MAXUnitConfig {
    
public:
    
    string _type;
    
    MAXUnitConfig(string balanceConfigName, string resourceConfigName);
    
};

class MAXUnitConfigManager {
    
    map<string, MAXUnitConfig*> _configs;
    
public:
    
    static MAXUnitConfigManager* SharedMAXUnitConfigManager();
    
    void LoadConfigsFromFile(string file);
    MAXUnitConfig* GetConfig(string type);
    
};

#endif /* defined(__MAX__MAXUnitConfig__) */
