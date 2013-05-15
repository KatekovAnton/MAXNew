//
//  GameUnitBaseParameters.h
//  MAX
//
//  Created by Anton Katekov on 15.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameUnitBaseParameters__
#define __MAX__GameUnitBaseParameters__

#include <iostream>

class MAXObjectConfig;
class MAXClanConfig;

class GameUnitBaseParameters {
    
public:
    
    GameUnitBaseParameters(MAXObjectConfig *unitConfig, MAXClanConfig *clanConfig);
    ~GameUnitBaseParameters();
    
};

#endif /* defined(__MAX__GameUnitBaseParameters__) */
