//
//  MAXLoadingController.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXLoadingController.h"
#include "MAXEngine.h"
#include "cocos2d.h"


MAXLoadingController::MAXLoadingController()
{
}

MAXLoadingController::~MAXLoadingController()
{
}

void MAXLoadingController::SetProgress(float zeroToOne)
{
	engine->DrawStart();
	engine->DrawInterface();
	engine->EndFrame();
}

