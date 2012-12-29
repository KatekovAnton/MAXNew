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
    
    this->SetMap("Green_6.wrl");
}

void MAXGame::SetMap(string mapName)
{
    engine->SetMap(MAXSCL->LoadMapWithName(mapName));
}

#pragma mark - DisplayPinchDelegate

bool MAXGame::CanStartPinch(float x, float y)
{
    float _y = Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale() - y;
    
    CCRect r = CCRect(0, 280, 130, 30);
    return !r.containsPoint(CCPoint(x,_y));
   // return _curretnState == MAXGAMESTATE_GAME;
}

void MAXGame::ProceedPinch(float scale)
{
    engine->ScaleCamera(scale);
}

void MAXGame::ProceedPan(float speedx, float speedy)
{
    engine->MoveCamera(speedx, speedy);
}

void MAXGame::ProceedTap(float tapx, float tapy)
{
    CCPoint p = engine->ScreenToWorldCoordinates(CCPoint(tapx, tapy));
    SysLogInfo("Tap coordinates: x=%f y=%f", p.x, p.y);
    
    engine->TestFire();
}

void MAXGame::ProceedLongTap(float tapx, float tapy)
{
}







