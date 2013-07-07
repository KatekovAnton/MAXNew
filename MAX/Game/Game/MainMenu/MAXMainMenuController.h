//
//  MAXMainMenuController.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXMainMenuController__
#define __MAX__MAXMainMenuController__

#include <iostream>

class ScreenMenu;

class MAXMainMenuController
{   

public:

	ScreenMenu *_sceneMenu;
	
    MAXMainMenuController();
    ~MAXMainMenuController();
    
    void Begin();
    void End();
   
};

#endif /* defined(__MAX__MAXMainMenuController__) */
