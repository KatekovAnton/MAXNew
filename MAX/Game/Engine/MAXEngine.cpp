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
	:_delegate(NULL), _applyedPaletteIndex(-100), _applyedPaletteCount(0), _map(NULL), _lastTexture(0)
{
    _renderSystem = new RenderSystem();
    _first = true;
    _lowLodHighDrawObjects = new USimpleContainer<MAXObject*>(100);
}

MAXEngine::~MAXEngine()
{
	SetMap(nullptr);
    
    delete _renderSystem;
	delete _lowLodHighDrawObjects;

	delete _camera;
    delete _animationManager;
    delete _grid;
    delete _unitSelection;
  
	delete _unitShader;
    delete _unitLowShader;
    delete _mapShader;
    delete _mapQuadShader;
    delete _resourceMapShader;
    delete _fogShader;
	delete _pathZoneShader;

}

void MAXEngine::Init() {
    
    SysInit();
    drawGrid = false;
    drawResources = false;
    drawFog = false;
    drawPathZone = false;
	_freezeAnimationManager = false;
    hidePathZoneCounter = 0;
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
    
	_director->setDisplayStats(false);
    _director->setContentScaleFactor(_renderSystem->GetDisplay()->GetDisplayScale());
	_grid = new MAXGrid();
    _unitSelection = new MAXUnitSelection();
    _statusRenderer = MAXStatusRenderer::SharedStatusRenderer();
    
	_scene = NULL;
    _resourceRenderer = NULL;
	_pathZoneRenderer = NULL;
    _fogRenderer = NULL;
	_optionalZoneRenderer = NULL;

    _pathZoneRendererLevel = OBJECT_LEVEL_UNDERWATER;
    _optionalZoneRendererLevel = OBJECT_LEVEL_OVERAIR;
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
	if (_map)
	{
		delete _map;
		_map = NULL;
	}
	if (map)
	{
		_map = new MAXMapObject(map);
		_grid->SetMapSize(_map->mapW, _map->mapH);
		_camera->SetMapSize(_map->mapW, _map->mapH);
	}
	if (_scene)
	{
        delete _scene;
		_scene = NULL;
	}
	if (_map)
		_scene = new SceneSystem(_map);
    
    
    if (_resourceRenderer)
	{
        delete _resourceRenderer;
		_resourceRenderer = NULL;
	}
	if (_map)
		_resourceRenderer = new MAXResourceMapRenderer(_map->mapW, _map->mapH);
    

    if (_fogRenderer)
    {
		delete _fogRenderer;
		_fogRenderer = NULL;
	}
	if (_map)
	{
		_fogRenderer = new MAXSolidTileRenderer(_map->mapW, _map->mapH);
		_fogRenderer->CompletlyFillMap();
		_fogRenderer->color = GLKVector4Make(0, 0, 0, 0.4);
	}


	if (_pathZoneRenderer)
	{
		delete _pathZoneRenderer;
		_pathZoneRenderer = NULL;
	}
	if (_map)
	{
		_pathZoneRenderer = new MAXSolidTileRenderer(_map->mapW, _map->mapH);
		_pathZoneRenderer->color = GLKVector4Make(0.3, 0.4, 0.3, 0.9);
	}


    if (_optionalZoneRenderer)
    {
		delete _optionalZoneRenderer;
		_optionalZoneRenderer = NULL;
	}
	if (_map)
	{
		_optionalZoneRenderer = new MAXSolidTileRenderer(_map->mapW, _map->mapH);
		_optionalZoneRenderer->color = GLKVector4Make(1, 0, 0, 0.9);
	}
	_lowLodHighDrawObjects->clear();
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
	if (_scene)
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

void MAXEngine::SetZoom(float zoom)
{
    _camera->Scale(_camera->scale/ zoom);
}

float MAXEngine::CameraZoom()
{
    return _camera->scale;
}

void MAXEngine::FillFog()
{
    _fogRenderer->CompletlyFillMap();
}

void MAXEngine::ClearResourceFog()
{
    _resourceRenderer->Clear();
}

void MAXEngine::AddPathZoneCell(const int x, const int y)
{
	_pathZoneRenderer->AddCell(x, y);
}

void MAXEngine::ClearPathZone()
{
	_pathZoneRenderer->Clear();
}

void MAXEngine::AddOptionalZoneCell(const int x, const int y)
{
	_optionalZoneRenderer->AddCell(x, y);
}

void MAXEngine::ClearOptionalZone()
{
    _optionalZoneRenderer->Clear();
}

void MAXEngine::SetOptionalZoneColor(const Color &c)
{
    _optionalZoneRenderer->color = GLKVector4Make((float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
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
    
    
    this->Update();
    if (_delegate) 
        _delegate->onFrame();
    
    this->Draw();
    
    
    this->EndFrame();
}

void MAXEngine::FinishLoading()
{
	_freezeAnimationManager = true;
	Update();
	_freezeAnimationManager = false;
}

void MAXEngine::Update()
{
    
    RequestManager::SharedRequestManager()->Flush();
	
	if (!_scene)
	{
		if (!_freezeAnimationManager)
			_animationManager->Update();
		return;
	}
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
	
	if (!_freezeAnimationManager)
		_animationManager->Update();
	if (!_scene)
		return;

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

void MAXEngine::DrawStart()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MAXEngine::Draw()
{
	DrawStart();
	if (_scene)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
    
		GLint prog;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
	
		DrawGround();
		glEnable(GL_BLEND);
		DrawGrid();
		DrawUnits();
	
		DrawResourceMap();
		DrawFog();
		_unitSelection->Draw();
		_statusRenderer->DrawCircles();
		glUseProgram(prog);
    }
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
	_map->Draw(_shader);
    glActiveTexture(GL_TEXTURE0);
}

void MAXEngine::DrawUnits()
{
    _applyedPaletteIndex = -100;
    _applyedPaletteCount = 0;
    const USimpleContainer<PivotObject*>* objects = _scene->GetVisibleObjects();
    bool drawedPathZone = false;
    bool drawedAttackhZone = false;
    _lowLodHighDrawObjects->clear();

    
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (lowRender)
    {
        DrawPathZone();
        drawedPathZone = true;
        drawedAttackhZone = true;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        _shader = _unitLowShader;
        glUseProgram(_shader->GetProgram());
        _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
        _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    
        MAXSCL->unitMesh->Bind();
        for (int i = 0; i < objects->GetCount(); i++)
        {
            MAXObject* object = (MAXObject*)objects->objectAtIndex(i);
            if (object->_drawInLowLod)
                _lowLodHighDrawObjects->addObject(object);
            else
                object->DrawLow(_shader);
        }
        MAXSCL->unitMesh->Unbind();
        
        
        if (_lowLodHighDrawObjects->GetCount() > 0)
        {
            _shader = _unitShader;
            glUseProgram(_shader->GetProgram());
            _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
            _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
            _applyedPaletteIndex = -100;
            
            MAXSCL->unitMesh->Bind();
            for (int i = 0; i < _lowLodHighDrawObjects->GetCount(); i++)
            {
                MAXObject* object = (MAXObject*)_lowLodHighDrawObjects->objectAtIndex(i);
                object->Draw(_shader);
            }
            MAXSCL->unitMesh->Unbind();
            
        }
        DrawOptionalZone();
    }
    else
    {
        _shader = _unitShader;
        glUseProgram(_shader->GetProgram());
        _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
        _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
        
        
        MAXSCL->unitMesh->Bind();
        for (int i = 0; i < objects->GetCount(); i++)
        {
            MAXUnitObject* object = (MAXUnitObject*)objects->objectAtIndex(i);
            OBJECT_LEVEL currentLevel =(OBJECT_LEVEL)object->params_w->_bLevel;
            //NOTE
            if (currentLevel > _pathZoneRendererLevel && drawPathZone && !drawedPathZone) {
                MAXSCL->unitMesh->Unbind();
                DrawPathZone();
                
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                _shader = _unitShader;
                glUseProgram(_shader->GetProgram());
                MAXSCL->unitMesh->Bind();
                drawedPathZone = true;
            }
            
            if (currentLevel > _optionalZoneRendererLevel && !drawedAttackhZone) {
                MAXSCL->unitMesh->Unbind();
                DrawOptionalZone();
                
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                _shader = _unitShader;
                glUseProgram(_shader->GetProgram());
                MAXSCL->unitMesh->Bind();
                drawedAttackhZone = true;
            }
//            if (currentLevel <= _optionalZoneRendererLevel && drawedAttackhZone) {
//                int a = 0;
//                a++;
//            }
            
            object->Draw(_shader);
            MAXStatusRenderer::SharedStatusRenderer()->DrawUnitStatus(object);
            
        }
        MAXSCL->unitMesh->Unbind();
    }
    glActiveTexture(GL_TEXTURE0);
    if (!drawedPathZone && drawPathZone) {
        DrawPathZone();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    if (!drawedAttackhZone) {
        DrawOptionalZone();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
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
    if (!drawPathZone || hidePathZoneCounter > 0)
        return;
    
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
	_shader = _pathZoneShader;
	glUseProgram(_shader->GetProgram());
	_shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
	_shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
	_pathZoneRenderer->Draw(_shader);
}

void MAXEngine::DrawOptionalZone()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_shader = _pathZoneShader;
	glUseProgram(_shader->GetProgram());
	_shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
	_shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
	_optionalZoneRenderer->Draw(_shader);
}

void MAXEngine::DrawInterface()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

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


