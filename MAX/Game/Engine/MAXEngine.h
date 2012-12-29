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
using namespace std;

class PivotObject;
class RenderSystem;
class Shader;
class SceneSystem;
class MAXCamera;

class LevelObject;
class MapObject;
class MAXContentMap;
class MAXGrid;
class MAXUnitObject;

class MAXEngine  {
    
    
    RenderSystem        *_renderSystem;
    double                _elapsedTime;
    double                _fullTime;
    
    
    SceneSystem         *_scene;
    
    Shader              *_shader;
    Shader              *_unitShader;
    Shader              *_mapShader;
    float             _color;
    
public:
    
    
    
    float displayw,displayh;
    bool drawGrid;
    
    shared_ptr<MAXUnitObject>           _unit;
    shared_ptr<MapObject>               _map;
    
    MAXCamera              *_camera;
    MAXGrid                *_grid;
    CCDirector          *_director;
    
    MAXEngine();
    ~MAXEngine();
    
    void Init();
    void EngineMain();
    
    void RunLoop(double delta);
    
    double ElapsedTime() const {return _elapsedTime;};
    double FullTime() const {return _fullTime;};
    
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
    
    void ScaleCamera(float deltaScale);
    void MoveCamera(float deltax, float deltay);
    
    void SetMap(shared_ptr<MAXContentMap> map);
    
    CCPoint ScreenToWorldCoordinates(const CCPoint &screen);
    CCPoint ScreenToWorldCell(const CCPoint &screen);
    CCPoint WorldCoordinatesToScreen(const CCPoint &screen);
    CCPoint WorldCoordinatesToScreenCocos(const CCPoint &world);
    CCRect ScreenToWorldRect();
    
    
    void TestFire();
};


extern MAXEngine *  engine;

#endif /* defined(__MAX__MAXEngine__) */
