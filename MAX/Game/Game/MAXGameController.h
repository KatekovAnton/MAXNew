//
//  MAXGameController.h
//  MAX
//
//  Created by Anton Katekov on 20.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXGameController__
#define __MAX__MAXGameController__

#include <iostream>

class GameUnit;

class MAXGameController {
    
public:
    
    GameUnit *_selectedUnit;

    
    
    MAXGameController();
    ~MAXGameController();
    
};

#endif /* defined(__MAX__MAXGameController__) */
