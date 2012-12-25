//
//  MAXEngine.h
//  MAX
//
//  Created by Anton Katekov on 25.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXEngine__
#define __MAX__MAXEngine__

#include <iostream>


#include "cocos2d.h"

using namespace cocos2d;

class RenderSystem;
class Shader;
class SceneSystem;
class MAXCamera;

class LevelObject;

class MAXEngine  {
    
    
    RenderSystem        *_renderSystem;
    float                _elapsedTime;
    float                _fullTime;
    
    
    SceneSystem         *_scene;
    
    Shader              *_shader;
    float             _color;
    
public:
    
    std::shared_ptr<LevelObject>         _cube;
    
    MAXCamera              *_camera;
    
    CCDirector          *_director;
    
    MAXEngine();
    ~MAXEngine();
    
    void Init();
    void EngineMain();
    
    void RunLoop(double delta);
    
    float ElapsedTime();
    
    void Update();
    void Draw();
    void DrawInterface();
    void EndFrame();
    
    
    Shader * GetShader();
   // void AddObject(const std::shared_ptr<PivotObject>& object, const std::shared_ptr<PivotObject>& parentObject);
    
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
    
    //test
    
    void switchLight();
    
};


extern MAXEngine *  engine;

#endif /* defined(__MAX__MAXEngine__) */
