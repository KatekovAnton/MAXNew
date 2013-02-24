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
    CCMenuItemSprite* _toggleScanButton;
    CCMenuItemSprite* _toggleRangeButton;
    CCMenuItemSprite* _toggleShotsButton;
    CCMenuItemSprite* _toggleStatusButton;
    bool _drawGrid;
    bool _drawScan;
    
    
public:
    
    bool GetDrawGrid() const {return _drawGrid;}
    
    GameInterface();
    ~GameInterface();
    
    void InitBaseInterface();
    
    void UpdateToggleGridButton();
    void UpdateToggleScanButton();
    
    void OnToggleGrid();
    void OnToggleScan();
};

#endif /* defined(__MAX__GameInteface__) */
