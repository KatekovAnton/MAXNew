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
#include "ScreenLoading.h"

MAXLoadingController::MAXLoadingController()
	:_screenLoading(NULL)
{
	
}

MAXLoadingController::~MAXLoadingController()
{
	if (_screenLoading)
		_screenLoading->autorelease();
}

void MAXLoadingController::SetProgress(float zeroToOne)
{
	if (_screenLoading)
		_screenLoading->SetProgress(zeroToOne);

	engine->DrawStart();
	engine->DrawInterface();
	engine->EndFrame();
}

void MAXLoadingController::Begin()
{
	if (!_screenLoading)
	{
		_screenLoading = new ScreenLoading();
		_screenLoading->init();
		_screenLoading->InitBaseInterface();
		CCDirector::sharedDirector()->pushScene(_screenLoading);
		SetProgress(0);
	}
}

void MAXLoadingController::End()
{}

