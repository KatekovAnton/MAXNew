//
//  MAXLoadingController.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXLoadingController__
#define __MAX__MAXLoadingController__

#include <iostream>

class ScreenLoading;

class MAXLoadingController
{

	ScreenLoading *_screenLoading;
    
public:
    
    MAXLoadingController();
    ~MAXLoadingController();

	void SetProgress(float zeroToOne);

    void Begin();
    void End();
      
};

#endif /* defined(__MAX__MAXLoadingController__) */
