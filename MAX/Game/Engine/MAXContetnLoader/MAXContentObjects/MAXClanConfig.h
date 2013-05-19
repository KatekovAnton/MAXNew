//
//  MAXClanConfig.h
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXClanConfig__
#define __MAX__MAXClanConfig__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "StringUtils.h"

using namespace std;

struct _MAXClanUnitParameters
{
    string _paramName;
    int _modValue;
};
typedef struct _MAXClanUnitParameters MAXClanUnitParameters;

class MAXClanConfig {
    
    int _number;
    string _name;
    string _engText;
    int _flags;
    
    map<string, vector<MAXClanUnitParameters>> _modifications;
    
    void SetConfigValue(string key, string value);
    
public:
    
    int GetNumber() const {return _number;};
    string GetName() const {return _name;};
    string GetEngText() const {return _engText;};
    int GetFlags() const {return _flags;};
    vector<MAXClanUnitParameters> GetModificationsForType(string type) {string lowType = toLower(type); return _modifications.count(lowType) == 1 ? _modifications[lowType]:vector<MAXClanUnitParameters>();};
    
    MAXClanConfig(string clanConfig, int number);
    ~MAXClanConfig();
    
};

#endif /* defined(__MAX__MAXClansConfig__) */
