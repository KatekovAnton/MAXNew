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
#include "ScreenSelectGameTypeDelegate.h"

class ScreenMenu;

class MAXMainMenuController : public ScreenSelectGameTypeDelegate
{   

public:

	ScreenMenu *_sceneMenu;
	
    MAXMainMenuController();
    ~MAXMainMenuController();
    
    void Begin();
    void End();
    
#pragma mark - ScreenSelectGameTypeDelegate

	virtual void OnTutorial();
	virtual void OnHotseat();
	virtual void OnMultiplayer();
	virtual void OnEditor();
	virtual void OnInfo();
   
   
};

#endif /* defined(__MAX__MAXMainMenuController__) */
