//
//  MAXEngine.cpp
//  MAX
//
//  Created by Anton Katekov on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXEngine.h"
#include <time.h>
#include <math.h>
#include "FileManger.h"
#include "Utils.h"

#include "cocos2d.h"

#include "RenderSystem.h"
#include "Shader.h"
#include "Display.h"
#include "Geometry.h"
#include "SceneSystem.h"
#include "MAXCamera.h"
#include "MAXContetnLoader.h"
#include "LevelObject.h"
#include "RenderObject.h"
#include "MAXUnitObject.h"
#include "MAXGrid.h"
#include "MapObject.h"

using namespace cocos2d;
//using namespace Kompex;

MAXEngine globalEngine;
MAXEngine * engine = &globalEngine;

MAXEngine::MAXEngine() {
    _renderSystem = new RenderSystem();
}

void MAXEngine::Init() {
    
    SysInit();
    drawGrid = false;
    _renderSystem->Init();
    _renderSystem->InitOpenGL();
    
    
    GRect2D _screenRect = GRect2DMake(0, 0, _renderSystem->GetDisplay()->GetDisplayWidth(), _renderSystem->GetDisplay()->GetDisplayHeight());
    _camera = new MAXCamera(_screenRect);
    
    
   
    
    _unitShader = new Shader("ShaderUnit.vsh", "ShaderUnit.fsh");
    _mapShader = new Shader("ShaderMap.vsh", "ShaderMap.fsh");
  //  _shader1 = new Shader("ShaderPostQuad.vsh", "ShaderPostQuad.fsh");
    
    _director = CCDirector::sharedDirector();
    _director->setContentScaleFactor(_renderSystem->GetDisplay()->GetDisplayScale());
    _director->setOpenGLView((CCEGLView *)(_renderSystem->GetDisplay()));
    
    float scale = _renderSystem->GetDisplay()->GetDisplayScale();
    _renderSystem->GetDisplay()->setDesignResolutionSize(_renderSystem->GetDisplay()->GetDisplayWidth()/scale, _renderSystem->GetDisplay()->GetDisplayHeight()/scale, kResolutionNoBorder);
    
    
    _scene = new SceneSystem();
    //_scene->AddObject(shared_ptr<LevelObject>(LevelObject::CreateUnitQuad()), true);
    _scene->AddObject(MAXSCL->CreateUnit("ALNTANK"), true);
    
    _scene->GetInterfaceManager()->Prepare();
    _director->pushScene(_scene->GetInterfaceManager()->GetGUISession());
    
    _director->setDisplayStats(true);
    _grid = new MAXGrid();
}

MAXEngine::~MAXEngine() {
    delete _renderSystem;
    delete _shader;
}

Shader * MAXEngine::GetShader() {
    return _shader;
}

void MAXEngine::RunLoop(double delta) {
    
    displayw = Display::currentDisplay()->GetDisplayWidth()/Display::currentDisplay()->GetDisplayScale();
    displayh = Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale();
    
    _elapsedTime = delta;
    _fullTime += _elapsedTime;
    
    glClearColor(_color, _color, _color, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    this->Update();
    GLint prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    this->Draw();
    glUseProgram(prog);
    
    this->DrawInterface();
    this->EndFrame();
}

void MAXEngine::switchLight() {
//    float abs = _color - 0.6;
//    if(abs < 0)
//        abs = -abs;
//    
//    if (abs<0.1 ) {
//        _color = 0;
//    } else {
//        _color = 0.6;
//    }
    drawGrid = !drawGrid;
}

void MAXEngine::EndFrame() {
    _renderSystem->EndFrame();
}

void MAXEngine::DrawInterface() {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    if(drawGrid)
        _grid->DrawGrid();
    _director->mainLoop();
    ccDrawCircle(CCPoint(100,100), 10,  1, 10, false);
   
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
}

void MAXEngine::Update() {
    
    _scene->BeginFrame();
    
    
    _scene->Frame(_elapsedTime);
    _map->Frame(_elapsedTime);

    _scene->EndFrame();
    _scene->UpdateScene();
    bool updategrid = _camera->changed;
    _camera->Update();
    _scene->CalculateVisbleObject();
    if(updategrid)
        _grid->UpdateInfo(false);
    
    _scene->AfterUpdate();
}

void MAXEngine::Draw() {
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    
    _shader = _mapShader;
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM2, 1.0/_map->mapTexH);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM1, 1.0/_map->mapTexW);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM3, _map->mapW);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM4, _map->mapH);
    _map.get()->Draw(_shader);

    glEnable(GL_BLEND);
    _shader = _unitShader;
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);

    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    const UContainer<PivotObject>* objects = _scene->GetVisibleObjects();
    for (int i = 0; i < objects->GetCount(); i++) 
        objects->objectAtIndex(i)->Draw(_shader);
    glDisable(GL_BLEND);
    
        
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
}

float MAXEngine::ElapsedTime() {
    return _elapsedTime;
}

void MAXEngine::applicationDidEnterBackground() {
    
}

bool MAXEngine::applicationDidFinishLaunching() {
    return true;
}

void MAXEngine::applicationWillEnterForeground() {
    
}

void MAXEngine::ScaleCamera(float deltaScale)
{
    _camera->Scale(deltaScale);
}

void MAXEngine::MoveCamera(float deltax, float deltay)
{
    _camera->Move(deltax, deltay);
}

void MAXEngine::SetMap(shared_ptr<MAXContentMap> map)
{
    _map = shared_ptr<MapObject>(new MapObject(map));
    _grid->SetMapSize(_map->mapW, _map->mapH);
}

CCPoint MAXEngine::ScreenToWorldCoordinates(CCPoint screen)
{
    CCPoint camcentercell;
    camcentercell.x = _map->mapW/2.0 - _camera->position.x;
    camcentercell.y = _camera->position.y + _map->mapH/2.0;
    
    CCPoint camcenterCoords;
    camcenterCoords.x = camcentercell.x * 64.0;
    camcenterCoords.y = camcentercell.y * 64.0;
    
    CCPoint screenSize;
    screenSize.x = displayw * _camera->scale;
    screenSize.y = displayh * _camera->scale;
    
    CCPoint ltp;
    ltp.x = camcenterCoords.x - screenSize.x;
    ltp.y = camcenterCoords.y - screenSize.y;
    
    float dx = screen.x/displayw;
    float dy = screen.y/displayh;
    
    CCPoint result;
    result.x = ltp.x + screenSize.x * dx * 2;
    result.y = ltp.y + screenSize.y * dy * 2;
    
    return result;
}

CCPoint MAXEngine::ScreenToWorldCell(CCPoint screen)
{
    CCPoint coords = ScreenToWorldCoordinates(screen);
    return CCPoint(coords.x/64.0, coords.y/64.0);
}

CCPoint MAXEngine::WorldCoordinatesToScreen(CCPoint world)
{
    CCPoint camcentercell;
    camcentercell.x = _map->mapW/2.0 - _camera->position.x;
    camcentercell.y = _camera->position.y + _map->mapH/2.0;
    
    CCPoint camcenterCoords;
    camcenterCoords.x = camcentercell.x * 64.0;
    camcenterCoords.y = camcentercell.y * 64.0;
    
    CCPoint screenSize;
    screenSize.x = displayw * _camera->scale;
    screenSize.y = displayh * _camera->scale;
    
    CCPoint result;
    result.x = 0.5 * displayw * (world.x - camcenterCoords.x + screenSize.x)/screenSize.x;
    result.y = 0.5 * displayh * (world.y - camcenterCoords.y + screenSize.y)/screenSize.y;
    
    return result;
}

CCPoint MAXEngine::WorldCoordinatesToScreenCocos(const CCPoint &world)
{
    CCPoint camcentercell;
    camcentercell.x = _map->mapW/2.0 - _camera->position.x;
    camcentercell.y = _camera->position.y + _map->mapH/2.0;
    
    CCPoint camcenterCoords;
    camcenterCoords.x = camcentercell.x * 64.0;
    camcenterCoords.y = camcentercell.y * 64.0;
    
    CCPoint screenSize;
    screenSize.x = displayw * _camera->scale;
    screenSize.y = displayh * _camera->scale;
    
    CCPoint result;
    result.x = 0.5 * displayw * (world.x - camcenterCoords.x + screenSize.x)/screenSize.x;
    result.y = displayh - 0.5 * displayh * (world.y - camcenterCoords.y + screenSize.y)/screenSize.y;
    
    return result;
}

CCRect MAXEngine::ScreenToWorldRect()
{
    CCPoint camcentercell;
    camcentercell.x = _map->mapW/2.0 - _camera->position.x;
    camcentercell.y = _camera->position.y + _map->mapH/2.0;
    
    CCPoint camcenterCoords;
    camcenterCoords.x = camcentercell.x * 64.0;
    camcenterCoords.y = camcentercell.y * 64.0;
    
    CCPoint screenSize;
    screenSize.x = displayw * _camera->scale;
    screenSize.y = displayh * _camera->scale;
    
    CCPoint ltp;
    ltp.x = camcenterCoords.x - screenSize.x;
    ltp.y = camcenterCoords.y - screenSize.y;
    
    CCRect result;
    result.origin = ltp;
    result.size.width = screenSize.x;
    result.size.height = screenSize.y;
    
    return result;
}
