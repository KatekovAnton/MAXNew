//
//  GameInteface.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GameInteface__
#define __MAX__GameInteface__

#include <iostream>
#include "cocos2d.h"
#include "MAXEngine.h"

using namespace cocos2d;

class GameInterface : public CCScene
{
    CCMenuItemSprite* _toggleGridButton;
    bool _drawGrid;
    
    
    
public:
    
    bool GetDrawGrid() const {return _drawGrid;}
    
    GameInterface();
    ~GameInterface();
    
    void InitBaseInterface();
    
    
    void OnToggleGrid();
    void UpdateToggleGridButton();
};

#endif /* defined(__MAX__GameInteface__) */
