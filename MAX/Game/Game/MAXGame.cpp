//
//  MAXGame.cpp
//  MAX
//
//  Created by  Developer on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXGame.h"
#include "SceneSystem.h"
#include "MAXEngine.h"
#include "Display.h"
#include "UserInterface.h"

#include "MAXContetnLoader.h"


MAXGame globalGame;
MAXGame * game = &globalGame;


MAXGame::MAXGame()
{
    _curretnState = MAXGAMESTATE_GAME;
}

MAXGame::~MAXGame()
{}

void MAXGame::Init()
{
    Display::currentDisplay()->SetPinchDelegate(this);
    
    this->SetMap("Crater_1.wrl");
}

void MAXGame::SetMap(string mapName)
{
    MAXSCL->LoadMapWithName(mapName);
}

#pragma mark - DisplayPinchDelegate

bool MAXGame::CanStartPinch()
{
    return _curretnState == MAXGAMESTATE_GAME;
}

void MAXGame::ProceedPinch(float scale)
{
    engine->ScaleCamera(scale);
}

void MAXGame::ProceedPan(float speedx, float speedy)
{
    engine->MoveCamera(speedx, speedy);
}