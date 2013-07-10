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
#include "MAXAnimationDelegate.h"

class ScreenMenu;
class ScreenProgressBar;
class MAXAnimationWait;

class MAXMainMenuController : public ScreenSelectGameTypeDelegate, public MAXAnimationDelegate
{   

	MAXAnimationWait *_animWaitForStart;

public:

	ScreenMenu *_sceneMenu;
	ScreenProgressBar *_loadingProgressBar;

    MAXMainMenuController();
    ~MAXMainMenuController();
    
    void Begin();
    void End();

	void SetLoadingProgress(float zeroToOne);
	void LoadingScreenDidAppear(ScreenProgressBar *screen);
    
#pragma mark - ScreenSelectGameTypeDelegate

	virtual void OnTutorial();
	virtual void OnHotseat();
	virtual void OnMultiplayer();
	virtual void OnEditor();
	virtual void OnInfo();
   
#pragma mark - MAXAnimationDelegate 
  
    virtual void OnAnimationStart(MAXAnimationBase* animation);
    virtual void OnAnimationUpdate(MAXAnimationBase* animation);
    virtual void OnAnimationFinish(MAXAnimationBase* animation);
    
   
};

#endif /* defined(__MAX__MAXMainMenuController__) */
