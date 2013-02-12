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
#include "MAXAnimationManager.h"
#include "MAXGrid.h"
#include "MAXUnitSelection.h"
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
#include "MAXMapObject.h"
#include "Request.h"
#include "RequestManager.h"
#include "Response.h"
#include "MAXDrawPrimitives.h"

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
    
    
    Request* r = new Request();
    r->AddHeader("Accept: application/json");
    r->AddHeader("charsets: utf-8");
    r->AddHeader("Content-Length: 0");
    r->AddHeader("sclub_image_type: 4");
    r->AddHeader("sclubclient_platform: ios");
    r->AddHeader("sclubclient_version: 10");
    
    r->SetUrl("http://api-dev-mobile.sclub.ru/partners/3/?token=c470727a82075a94648cee86b116aecb");
    r->SetMethod(RequestMethod::GET);
    r->delegate = this;
    r->_needAutoDelete = true;
    RequestManager::SharedRequestManager()->ExecuteRequest(r);
    
    
    _unitShader = new Shader("ShaderUnit.vsh", "ShaderUnit.fsh");
    _mapShader = new Shader("ShaderMap.vsh", "ShaderMap.fsh");
    MAXDrawPrimitives::SharedDrawPrimitives();

  //  _shader1 = new Shader("ShaderPostQuad.vsh", "ShaderPostQuad.fsh");
    GCCHECK_GL_ERROR_DEBUG(); 

        
    _director = CCDirector::sharedDirector();
    _director->setContentScaleFactor(_renderSystem->GetDisplay()->GetDisplayScale());
    _director->setOpenGLView((CCEGLView *)(_renderSystem->GetDisplay()));
      
    float scale = _renderSystem->GetDisplay()->GetDisplayScale();
    _renderSystem->GetDisplay()->setDesignResolutionSize(_renderSystem->GetDisplay()->GetDisplayWidth()/scale, _renderSystem->GetDisplay()->GetDisplayHeight()/scale, kResolutionNoBorder);
    _animationManager = new MAXAnimationManager();
    
    _scene = new SceneSystem();
    
    _director->setDisplayStats(true);
    _grid = new MAXGrid();
    _unitSelection = new MAXUnitSelection();
}

void MAXEngine::AddUnit(const shared_ptr<MAXUnitObject>& newUnit)
{
    _scene->AddObject(newUnit, true);
}

void MAXEngine::RemoveUnit(const shared_ptr<MAXUnitObject>& newUnit)
{
    _scene->RemoveObject(newUnit);
}

MAXEngine::~MAXEngine() {
    delete _animationManager;
    delete _renderSystem;
    delete _shader;
    delete _mapShader;
    delete _grid;
    delete _scene;
    delete _unitSelection;
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
    this->Draw();
    
    
    this->EndFrame();
}

void MAXEngine::Update()
{
    RequestManager::SharedRequestManager()->Flush();
    _scene->BeginFrame();
    
    
    _scene->Frame(_elapsedTime);
    _map->Frame(_elapsedTime);
    
    _scene->EndFrame();
    _scene->UpdateScene();
    bool updategrid = _camera->changed;
    _camera->Update();
    _scene->CalculateVisbleObject();
    _animationManager->Update();
    if(updategrid)
    {
        _grid->cameraScale = _camera->scale;
        _grid->UpdateInfo(false);
    }
    _scene->AfterUpdate();
    _unitSelection->Update();
}

void MAXEngine::SelectUnit(const shared_ptr<MAXUnitObject>& unit)
{
    _unitSelection->SelectUnit(unit);
}

void MAXEngine::DrawLine()
{
//    MAXDrawPrimitives::SharedDrawPrimitives()->Begin();
//    
//    MAXDrawPrimitives::SharedDrawPrimitives()->_color = GLKVector4Make(1, 1, 1, 1);
//    MAXDrawPrimitives::SharedDrawPrimitives()->BindColor();
//    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(ccp(10, 10), ccp(100, 100));
//    
//    MAXDrawPrimitives::SharedDrawPrimitives()->_color = GLKVector4Make(1, 1, 0, 1);
//    MAXDrawPrimitives::SharedDrawPrimitives()->BindColor();
//    MAXDrawPrimitives::SharedDrawPrimitives()->DrawLine(ccp(100, 100), ccp(30, 320));
//    
//    MAXDrawPrimitives::SharedDrawPrimitives()->_color = GLKVector4Make(1, 1, 0, 1);
//    MAXDrawPrimitives::SharedDrawPrimitives()->BindColor();
//    MAXDrawPrimitives::SharedDrawPrimitives()->DrawCircle(CCPoint(100,100), 140,  1, 40, false);
}

void MAXEngine::Draw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    GLint prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    DrawGround();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    DrawGrid();
    DrawUnits();
    _unitSelection->Draw();
    glUseProgram(prog);
    glEnable(GL_DEPTH_TEST);
    DrawInterface();
}

void MAXEngine::DrawGrid()
{
    if(drawGrid)
        _grid->DrawGrid();
}

void MAXEngine::DrawGround()
{
    _shader = _mapShader;
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM2, 1.0/_map->mapTexH);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM1, 1.0/_map->mapTexW);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM3, _map->mapW);
    _shader->SetFloatValue(UNIFORM_FLOATPARAM4, _map->mapH);
    _map.get()->Draw(_shader);
    
    glActiveTexture(GL_TEXTURE0);
}

void MAXEngine::DrawUnits()
{
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    _shader = _unitShader;
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
    
    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    const UContainer<PivotObject>* objects = _scene->GetVisibleObjects();
    for (int i = 0; i < objects->GetCount(); i++)
        objects->objectAtIndex(i)->Draw(_shader);
 
    glActiveTexture(GL_TEXTURE0);
}

void MAXEngine::DrawInterface()
{
    _director->mainLoop();
}

void MAXEngine::EndFrame()
{
    _renderSystem->EndFrame();
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
    _map = shared_ptr<MAXMapObject>(new MAXMapObject(map));
    _grid->SetMapSize(_map->mapW, _map->mapH);
    _camera->SetMapSize(_map->mapW, _map->mapH);
}

CCPoint MAXEngine::ScreenToWorldCoordinates(const CCPoint &screen)
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

CCPoint MAXEngine::ScreenToWorldCell(const CCPoint &screen)
{
    CCPoint coords = ScreenToWorldCoordinates(screen);
    return CCPoint(coords.x/64.0, coords.y/64.0);
}

CCPoint MAXEngine::WorldCoordinatesToScreen(const CCPoint &world)
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
    result.size.width = screenSize.x * 2.0;
    result.size.height = screenSize.y * 2.0;
    
    return result;
}

#pragma mark - RequestDelegate
void MAXEngine::RequestDidFinishLoadingWithResponce(Request* request, Response* response)
{
//    printf("responce: \n");
//    printf("%s", response->ToString().c_str());
//    printf("\n");
}

