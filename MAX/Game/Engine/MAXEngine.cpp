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

#include "miniPrefix.h"
#include "cocos2d.h"
#include "EngineMesh.h"
#include "Framebuffer.h"
#include "RenderSystem.h"
#include "Shader.h"
#include "Display.h"
#include "Geometry.h"
#include "SceneSystem.h"
#include "MAXCamera.h"
#include "MAXContentLoader.h"
#include "LevelObject.h"
#include "RenderObject.h"
#include "MAXUnitObject.h"
#include "MAXMapObject.h"
#include "Request.h"
#include "RequestManager.h"
#include "Response.h"
#include "MAXDrawPrimitives.h"
#include "MAXStatusRenderer.h"
#include "MAXResourceMapRenderer.h"
#include "MAXSolidTileRenderer.h"

using namespace cocos2d;
//using namespace Kompex;

MAXEngine globalEngine;
MAXEngine * engine = &globalEngine;

MAXEngine::MAXEngine()
	:_delegate(NULL), _applyedPaletteIndex(-100), _applyedPaletteCount(0)
{
    _renderSystem = new RenderSystem();
    _first = true;
}

MAXEngine::~MAXEngine()
{
    if (_fogRenderer) {
        delete _fogRenderer;
        _fogRenderer = NULL;
    }
	if (_pathZoneRenderer)
	{
		delete _pathZoneRenderer;
		_pathZoneRenderer = NULL;
	}
	

    if (_resourceRenderer) {
        delete _resourceRenderer;
        _resourceRenderer = NULL;
    }
    
    delete _animationManager;
    delete _renderSystem;
  
    delete _mapShader;
    delete _mapQuadShader;
    delete _grid;
    delete _scene;
    delete _unitSelection;
}

void MAXEngine::Init() {
    
    SysInit();
    drawGrid = false;
    drawResources = false;
    drawFog = false;
    drawPathZone = false;
    _renderSystem->Init();
    _renderSystem->InitOpenGL();
    
    GRect2D _screenRect = GRect2DMake(0, 0, _renderSystem->GetDisplay()->GetDisplayWidth()/_renderSystem->GetDisplay()->GetDisplayScale(), _renderSystem->GetDisplay()->GetDisplayHeight()/_renderSystem->GetDisplay()->GetDisplayScale());
    _camera = new MAXCamera(_screenRect,1.0);
    
   
    
    _unitShader = new Shader("ShaderUnit.vsh", "ShaderUnit.fsh");
    _unitLowShader = new Shader("ShaderUnitLow.vsh", "ShaderUnitLow.fsh");
    _mapShader = new Shader("ShaderMap.vsh", "ShaderMap.fsh");
    _mapQuadShader = new Shader("ShaderPostQuad.vsh", "ShaderPostQuad.fsh");
    _resourceMapShader = new Shader("ShaderResourceMap.vsh", "ShaderResourceMap.fsh");
    _fogShader = new Shader("ShaderSolidFog.vsh", "ShaderSolidFog.fsh");
	_pathZoneShader = new Shader("ShaderSolidFog.vsh", "ShaderSolidFog.fsh");
    _mapQuadMesh = EngineMesh::CreateScaledQuad(2,2);
    MAXDrawPrimitives::SharedDrawPrimitives();

    GCCHECK_GL_ERROR_DEBUG(); 

        
    _director = CCDirector::sharedDirector();
    _director->setContentScaleFactor(_renderSystem->GetDisplay()->GetDisplayScale());
    _director->setOpenGLView((CCEGLView *)(_renderSystem->GetDisplay()));

	
    if (Display::currentDisplay()->GetDisplayScale() == 1.0) {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("simple");
    }
    if (Display::currentDisplay()->GetDisplayScale() == 2.0) {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("retina");
    }
      
    float scale = _renderSystem->GetDisplay()->GetDisplayScale();
    _renderSystem->GetDisplay()->setDesignResolutionSize(_renderSystem->GetDisplay()->GetDisplayWidth()/scale, _renderSystem->GetDisplay()->GetDisplayHeight()/scale, kResolutionNoBorder);
    _animationManager = new MAXAnimationManager();
    
    
    _director->setDisplayStats(true);
    _grid = new MAXGrid();
    _unitSelection = new MAXUnitSelection();
    _statusRenderer = MAXStatusRenderer::SharedStatusRenderer();
    _scene = NULL;
    _resourceRenderer = NULL;
	_pathZoneRenderer = NULL;
    _fogRenderer = NULL;
}

void MAXEngine::SetCameraCenter(const CCPoint &cell)
{
    if (!_map) {
        return;
    }
    CCPoint result = ccp(_map->mapH/2, _map->mapW/2);
    result.x -= cell.x;
    result.y -= cell.y;
    
    result.x += 1;
    result.y = -result.y;
    
    _camera->SetPosition(GLKVector3Make(result.x, result.y, 0));
}

void MAXEngine::SetMap(shared_ptr<MAXContentMap> map)
{
    
    _map = shared_ptr<MAXMapObject>(new MAXMapObject(map));
    _grid->SetMapSize(_map->mapW, _map->mapH);
    _camera->SetMapSize(_map->mapW, _map->mapH);
    if (_scene)
        delete _scene;
    _scene = new SceneSystem(_map.get());
    
    
    if (_resourceRenderer)
        delete _resourceRenderer;
    _resourceRenderer = new MAXResourceMapRenderer(_map->mapW, _map->mapH);
    
    if (_fogRenderer)
        delete _fogRenderer;
    _fogRenderer = new MAXSolidTileRenderer(_map->mapW, _map->mapH);
    _fogRenderer->CompletlyFillMap();
    _fogRenderer->color = GLKVector4Make(0, 0, 0, 0.4);

	if (_pathZoneRenderer)
	{
		delete _pathZoneRenderer;
	}
	_pathZoneRenderer = new MAXSolidTileRenderer(_map->mapW, _map->mapH);
	_pathZoneRenderer->color = GLKVector4Make(0., 0.8, 0., 0.2);
}

void MAXEngine::ClearMap()
{
    if(_resourceRenderer)
        delete _resourceRenderer;
    _resourceRenderer = NULL;
    if (_scene)
        delete _scene;
    _scene = NULL;
    _map = nullptr;
}

void MAXEngine::AddUnit(PivotObject* newUnit)
{
    _scene->AddObject(newUnit, true);
    newUnit->HasBeenLocatedToScene();
}

void MAXEngine::RemoveUnit(PivotObject* newUnit)
{
    _scene->RemoveObject(newUnit);
    newUnit->HasBeenRemovedFromScene();
}

void MAXEngine::AddResourceCell(const int x, const int y, const RESOURCE_TYPE type, const unsigned char amount)
{
    if (!_resourceRenderer)
        return;
    
    _resourceRenderer->AddCellToScan(x, y, type, amount);
}

void MAXEngine::AddFogCell(const int x, const int y, const bool fog)
{
    if (!_fogRenderer) 
        return;
    
//    if (x==86 && y == 99) {
//        int a = 0;
//        a++;
//    }
    
    if (fog)
        _fogRenderer->AddCell(x, y);
    else
        _fogRenderer->RemoveCell(x, y);
}

void MAXEngine::SetPathZoneLevel(OBJECT_LEVEL level)
{
	//_pathZoneRenderer->AddCell(x, y);
}

void MAXEngine::AddPathZoneCell(const int x, const int y)
{
	_pathZoneRenderer->AddCell(x, y);
}

void MAXEngine::ClearPathZone()
{
	_pathZoneRenderer->Clear();
}

void MAXEngine::GetAllObjectsInArea(BoundingBox bb, USimpleContainer<MAXObject*> *buffer)
{
    _scene->GetAllObjectsInArea(bb, (USimpleContainer<PivotObject*>*)buffer);
}

Shader * MAXEngine::GetShader()
{
    return _shader;
}

void MAXEngine::RunLoop(double delta)
{
    
    displayw = Display::currentDisplay()->GetDisplayWidth()/Display::currentDisplay()->GetDisplayScale();
    displayh = Display::currentDisplay()->GetDisplayHeight()/Display::currentDisplay()->GetDisplayScale();
    
    _elapsedTime = delta;
    _fullTime += _elapsedTime;
    
    glClearColor(_color, _color, _color, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    this->Update();
    if (_delegate) 
        _delegate->onFrame();
    
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
    if (_first) {
        _camera->Move(0, 0);
        _first = false;
    }
    bool updategrid = _camera->changed;
    _camera->Update();
    _animationManager->Update();
    if(updategrid)
    {
        _grid->cameraScale = _camera->scale;
        _grid->UpdateInfo(false);
    }
    _scene->AfterUpdate();
    _scene->CalculateVisbleObject();
    
    CCPoint c1 = engine->WorldCoordinatesToScreenCocos(ccp(0, 0));
    CCPoint c2 = engine->WorldCoordinatesToScreenCocos(ccp(64, 0));
    float oneCellRadius = c2.x - c1.x;
    lowRender = oneCellRadius < 10 || _camera->minZoom;
    if (!lowRender) {
        USimpleContainer<PivotObject*> *container = _scene->GetVisibleObjects();
        if (container->GetCount()!=0)
            container->sort(container->objectAtIndex(0)->GetCompareFunc());
    }
    _scene->LastUpdate(lowRender);
    _unitSelection->Update();
}

void MAXEngine::SelectUnit(MAXObject* unit)
{
    _unitSelection->SelectUnit(unit);
}

void MAXEngine::Draw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    GLint prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    DrawGround();
    glEnable(GL_BLEND);
    DrawGrid();
    DrawUnits(false);
	DrawPathZone();
    DrawUnits(true);
    DrawResourceMap();
    DrawFog();
    _unitSelection->Draw();
    _statusRenderer->DrawCircles();
    glUseProgram(prog);
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
    _map.get()->Draw(_shader);
    glActiveTexture(GL_TEXTURE0);
}

void MAXEngine::DrawUnits(bool highLevel)
{
#if !DRAW_UNIT_HI_LO
    if (!highLevel)
        return;
#endif
    _applyedPaletteIndex = -100;
    _applyedPaletteCount = 0;

    
    
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (lowRender)
    {
        _shader = _unitLowShader;
        glUseProgram(_shader->GetProgram());
        _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
        _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
        const USimpleContainer<PivotObject*>* objects = _scene->GetVisibleObjects();
        
        MAXSCL->unitMesh->Bind();
        for (int i = 0; i < objects->GetCount(); i++)
            objects->objectAtIndex(i)->DrawLow(_shader);
        MAXSCL->unitMesh->Unbind();
    }
    else
    {
        _shader = _unitShader;
        glUseProgram(_shader->GetProgram());
        _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
        _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
        const USimpleContainer<PivotObject*>* objects = _scene->GetVisibleObjects();
        
        OBJECT_LEVEL level = OBJECT_LEVEL_UNDERWATER;
        
        MAXSCL->unitMesh->Bind();
        for (int i = 0; i < objects->GetCount(); i++)
        {
            MAXUnitObject* object = (MAXUnitObject*)objects->objectAtIndex(i);
#if DRAW_UNIT_HI_LO
            if (highLevel == object->_highLevel)
#endif
            {
                object->Draw(_shader);
                MAXStatusRenderer::SharedStatusRenderer()->DrawUnitStatus(object);
            }
            level = (OBJECT_LEVEL)object->params_w->_bLevel;
        }
        MAXSCL->unitMesh->Unbind();
    }
    glActiveTexture(GL_TEXTURE0);
   // printf("%d\n",_applyedPaletteCount);
}

void MAXEngine::DrawResourceMap()
{
    if (!drawResources) 
        return;
    _shader = _resourceMapShader;
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    _resourceRenderer->Draw(_shader);
}

void MAXEngine::DrawFog()
{
    if (!drawFog) 
        return;

    _shader = _fogShader;
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    _fogRenderer->Draw(_shader);
}

void MAXEngine::DrawPathZone()
{
    if (!drawPathZone) 
        return;
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_shader = _pathZoneShader;
	glUseProgram(_shader->GetProgram());
	_shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
	_shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
	_pathZoneRenderer->Draw(_shader);
}

void MAXEngine::DrawInterface()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

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

GLKVector2 MAXEngine::WorldCoordinatesToScreenCocosV(const GLKVector2 &world)
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
    
    GLKVector2 result;
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

