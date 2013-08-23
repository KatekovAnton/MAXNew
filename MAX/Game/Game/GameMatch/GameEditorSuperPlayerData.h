//
//  GameEditorSuperPlayerData.h
//  MAX
//
//  Created by  Developer on 24.08.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameEditorSuperPlayerData__
#define __MAX__GameEditorSuperPlayerData__

#include <iostream>
#include "GameMatchPlayerData.h"

class GameEditorSuperPlayerData : public GameMatchPlayerData {
    
public:
    
    GameEditorSuperPlayerData(GameMatchPlayerInfo playerInfo, GameMatch *match);
    void Initialize();
    
    
    virtual bool CanSeeUnit(GameUnitData* unit);
    
};

#endif /* defined(__MAX__GameEditorSuperPlayerData__) */
