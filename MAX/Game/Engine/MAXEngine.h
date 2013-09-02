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
#include "miniPrefix.h"

using namespace Utils;
using namespace cocos2d;
using namespace std;

class EngineMesh;
class Framebuffer;
class PivotObject;
class MAXObject;
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
class MAXResourceMapRenderer;
class MAXSolidTileRenderer;

class MAXEngineDelegate {
public:
    virtual void onFrame() = 0;
};

class MAXEngine {
    bool _first;
    
    RenderSystem        *_renderSystem;
    double                _elapsedTime;
    double                _fullTime;
    
    bool _freezeAnimationManager;
    MAXAnimationManager *_animationManager;
    SceneSystem         *_scene;
    
    Shader              *_shader;
    Shader              *_unitShader;
    Shader              *_unitLowShader;
    Shader              *_mapShader;
    Shader              *_mapQuadShader;
    Shader              *_resourceMapShader;
    Shader              *_fogShader;
	Shader				*_pathZoneShader;
    shared_ptr<EngineMesh> _mapQuadMesh;
    
    bool lowRender;
    OBJECT_LEVEL _pathZoneRendererLevel;
    OBJECT_LEVEL _optionalZoneRendererLevel;
    
    USimpleContainer<MAXObject*> *_lowLodHighDrawObjects;
    
public:
    
    unsigned int _lastTexture;
    
    MAXResourceMapRenderer  *_resourceRenderer;
    MAXSolidTileRenderer    *_fogRenderer;
	MAXSolidTileRenderer    *_pathZoneRenderer;
	MAXSolidTileRenderer    *_optionalZoneRenderer;
    
    
    MAXEngineDelegate   *_delegate;
    
    MAXAnimationManager* GetAnimationManager () const { return _animationManager; };
    
    float displayw,displayh;
    bool drawGrid;
    bool drawResources;
    bool drawFog;
    bool drawPathZone;
    int hidePathZoneCounter;
    
    int _applyedPaletteIndex;
    int _applyedPaletteCount;
    
    MAXMapObject			*_map;
    
    MAXCamera               *_camera;
    MAXGrid                 *_grid;
    MAXUnitSelection        *_unitSelection;
    MAXStatusRenderer       *_statusRenderer;
    CCDirector              *_director;
    
    MAXEngine();
    ~MAXEngine();
    
    void Init();
    
    void GetAllObjectsInArea(BoundingBox bb, USimpleContainer<MAXObject*> *buffer);
    
    void SelectUnit(MAXObject* unit);
    void AddUnit(PivotObject* newUnit);
    void RemoveUnit(PivotObject* newUnit);
    
    void AddFogCell(const int x, const int y, const bool fog);
    void FillFog();
    
    void AddResourceCell(const int x, const int y, const RESOURCE_TYPE type, const unsigned char amount);
    void ClearResourceFog();
    
	void AddPathZoneCell(const int x, const int y);
    void SetPathZoneLevel(OBJECT_LEVEL level) { _pathZoneRendererLevel = level; };
	void ClearPathZone();
    
    void AddOptionalZoneCell(const int x, const int y);
    void SetOptionalZoneLevel(OBJECT_LEVEL level) { _optionalZoneRendererLevel = level; };
	void ClearOptionalZone();
    void SetOptionalZoneColor(const Color &c);
    
    
    double ElapsedTime() const {return _elapsedTime;};
    double FullTime() const {return _fullTime;};
    void RunLoop(double delta);
    
	
    void SetCameraCenter(const CCPoint &cell);
    void SetZoom(float zoom);
    float CameraZoom();
    
    void Update();
	void DrawStart();
    void Draw();
    void DrawGround();
    void DrawUnits();
    void DrawResourceMap();
    void DrawFog();
	void DrawPathZone();
	void DrawOptionalZone();
    void DrawGrid();
    void DrawInterface();
    void EndFrame();
    
    void FinishLoading();
    
    Shader * GetShader();
   // void AddObject(const std::shared_ptr<PivotObject>& object, const std::shared_ptr<PivotObject>& parentObject);
    
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

    
    void ScaleCamera(float deltaScale);
    void MoveCamera(float deltax, float deltay);
    
    void SetMap(shared_ptr<MAXContentMap> map);
    void ClearMap();
    
    CCPoint ScreenToWorldCoordinates(const CCPoint &screen);
    CCPoint ScreenToWorldCell(const CCPoint &screen);
    CCPoint WorldCoordinatesToScreen(const CCPoint &screen);
    CCPoint WorldCoordinatesToScreenCocos(const CCPoint &world);
    GLKVector2 WorldCoordinatesToScreenCocosV(const GLKVector2 &world);
    CCRect ScreenToWorldRect();
    
};


extern MAXEngine *  engine;

#endif /* defined(__MAX__MAXEngine__) */
