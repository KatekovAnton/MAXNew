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
#include "RequestDelegate.h"


using namespace cocos2d;
using namespace std;

class EngineMesh;
class Framebuffer;
class PivotObject;
class RenderSystem;
class Shader;
class SceneSystem;
class MAXCamera;
class MAXAnimationManager;
class MAXGrid;
class MAXUnitSelection;

class MAXMapObject;
class MAXContentMap;
class MAXUnitObject;
class MAXStatusRenderer;

class MAXEngine : public RequestDelegate  {
    
    
    RenderSystem        *_renderSystem;
    double                _elapsedTime;
    double                _fullTime;
    
    MAXAnimationManager *_animationManager;
    SceneSystem         *_scene;
    
    Shader              *_shader;
    Shader              *_unitShader;
    Shader              *_mapShader;
    Shader              *_mapQuadShader;
    shared_ptr<EngineMesh> _mapQuadMesh;
    float             _color;
public:
    
    MAXAnimationManager* GetAnimationManager () const { return _animationManager; };
    
    float displayw,displayh;
    bool drawGrid;
    
    
    
    shared_ptr<MAXMapObject>            _map;
    
    MAXCamera               *_camera;
    MAXGrid                 *_grid;
    MAXUnitSelection        *_unitSelection;
    MAXStatusRenderer       *_statusRenderer;
    CCDirector              *_director;
    
    MAXEngine();
    ~MAXEngine();
    
    void Init();
    void EngineMain();
    
    void SelectUnit(MAXUnitObject* unit);
    void AddUnit(MAXUnitObject* newUnit);
    void RemoveUnit(MAXUnitObject* newUnit);
    
    void RunLoop(double delta);
    
    double ElapsedTime() const {return _elapsedTime;};
    double FullTime() const {return _fullTime;};
    
    void Update();
    void Draw();
    void DrawLine();
    void DrawGround();
    void DrawUnits();
    void DrawGrid();
    void DrawInterface();
    void EndFrame();
    
    
    
    Shader * GetShader();
   // void AddObject(const std::shared_ptr<PivotObject>& object, const std::shared_ptr<PivotObject>& parentObject);
    
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

    
    void ScaleCamera(float deltaScale);
    void MoveCamera(float deltax, float deltay);
    
    void SetMap(shared_ptr<MAXContentMap> map);
    
    CCPoint ScreenToWorldCoordinates(const CCPoint &screen);
    CCPoint ScreenToWorldCell(const CCPoint &screen);
    CCPoint WorldCoordinatesToScreen(const CCPoint &screen);
    CCPoint WorldCoordinatesToScreenCocos(const CCPoint &world);
    CCRect ScreenToWorldRect();
    
#pragma mark - RequestDelegate
    void RequestDidFinishLoadingWithResponce(Request* request, Response* response);
};


extern MAXEngine *  engine;

#endif /* defined(__MAX__MAXEngine__) */
