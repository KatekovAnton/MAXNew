//
//  ScreenLoading.h
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenLoading__
#define __MAX__ScreenLoading__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class ScreenLoading : public CCScene
{

	CCLayerColor *_layerBg;
	CCLayerColor *_layerFrame;
	CCLayerColor *_layerProgressBar;

public:  
    
	ScreenLoading();
    ~ScreenLoading();

    void InitBaseInterface();
    
	void SetProgress(float zeroToOne);

};

#endif /* defined(__MAX__ScreenLoading__) */
