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
#include "MAXGame.h"
#include "MAXCamera.h"
#include "LevelObject.h"
#include "RenderObject.h"

using namespace cocos2d;
using namespace Kompex;

MAXEngine globalEngine;
MAXEngine * engine = &globalEngine;

MAXEngine::MAXEngine() {
    _renderSystem = new RenderSystem();
}

void MAXEngine::Init() {
    
    SysInit();
    
    
    _renderSystem->Init();
    _renderSystem->InitOpenGL();
    
    GRect2D _screenRect = GRect2DMake(0, 0, _renderSystem->GetDisplay()->GetDisplayWidth(), _renderSystem->GetDisplay()->GetDisplayHeight());
    _camera = new MAXCamera(_screenRect);
    
    
    _cube = shared_ptr<LevelObject>(LevelObject::CreateUnitQuad());
    
    
    _shader = new Shader("ShaderBackground.vsh", "ShaderBackground.fsh");
  //  _shader1 = new Shader("ShaderPostQuad.vsh", "ShaderPostQuad.fsh");
    
    _director = CCDirector::sharedDirector();
    _director->setOpenGLView((CCEGLView *)(_renderSystem->GetDisplay()));
    
    _renderSystem->GetDisplay()->setDesignResolutionSize(_renderSystem->GetDisplay()->GetDisplayWidth(), _renderSystem->GetDisplay()->GetDisplayHeight(), kResolutionNoBorder);
    _director->setContentScaleFactor(2.0f);
    
    
    _scene = new SceneSystem();
    
    _scene->GetInterfaceManager()->Prepare();
    _director->pushScene(_scene->GetInterfaceManager()->GetGUISession());
    
    _director->setDisplayStats(true);
}

Shader * MAXEngine::GetShader() {
    return _shader;
}

void MAXEngine::RunLoop(double delta) {
    _elapsedTime = delta;
    _fullTime += _elapsedTime;
    
    glClearColor(_color, _color, _color, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    this->Update();
    this->Draw();
    
    this->DrawInterface();
    this->EndFrame();
}

void MAXEngine::switchLight() {
    float abs = _color - 0.6;
    if(abs < 0)
        abs = -abs;
    
    if (abs<0.1 ) {
        _color = 0;
    } else {
        _color = 0.6;
    }
}

void MAXEngine::EndFrame() {
    _renderSystem->EndFrame();
}

void MAXEngine::DrawInterface() {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    _director->mainLoop();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void MAXEngine::Update() {
    
    _scene->BeginFrame();
    
    
    _scene->Frame(_elapsedTime);
    

    _scene->EndFrame();
    _scene->UpdateScene();
    _camera->Update();
}

void MAXEngine::Draw() {
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    
    
    glUseProgram(_shader->GetProgram());
    _shader->SetMatrixValue(UNIFORM_VIEW_MATRIX, _camera->view.m);
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            printf(" glError: 0x%04X", err);
    }

    _shader->SetMatrixValue(UNIFORM_PROJECTION_MATRIX, _camera->projection.m);
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            printf(" glError: 0x%04X", err);
    }
    GLKMatrix4 m1 = _cube->GetTransformMatrix();
    //printf("x: %f y: %f z: %f\n", m1.m30, m1.m31, m1.m32);
    _shader->SetMatrixValue(UNIFORM_MODEL_MATRIX, m1.m);
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            printf(" glError: 0x%04X", err);
    }
    
    _cube->GetRenderAspect()->Render(0, _cube->GetMaterial());
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            printf(" glError: 0x%04X", err);
    }
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

MAXEngine::~MAXEngine() {
    delete _renderSystem;
    delete _shader;
}

void MAXEngine::ScaleCamera(float deltaScale)
{
    _camera->Scale(deltaScale);
}

void MAXEngine::MoveCamera(float deltax, float deltay)
{
    _camera->Move(deltax, deltay);
}
