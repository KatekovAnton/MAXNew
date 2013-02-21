//
//  PivotObject.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/31/12.
//
//

#ifndef __TerminalExtraction__PivotObject__
#define __TerminalExtraction__PivotObject__

#include <iostream>
#include "Geometry.h"

typedef int (*compareFunc)( const void *, const void * );

using namespace std;

class EditorData;
class ObjectBehaviourModel;
class RenderMatrixController;
class RenderObject;
class Material;
class Shader;

class PivotObject {
    
    double _sceneLocationTime;
    bool                _isOnScreen;
    
protected:
    
    GLKMatrix4              _transformMatrix;
    unsigned _objectId;
    
    
public:
    
    GLKVector2  _bbsize;
    BoundingBox _boundingShape;

    bool GetIsOnScreen() const {return _isOnScreen;};
    
    unsigned GetObjectId() const {return _objectId; };
    void SetObjectId(unsigned newValue) { _objectId = newValue; };
    
    bool        moved;
    bool        forceHidden;
    
    PivotObject();
    virtual ~PivotObject();

    GLKMatrix4 GetTransformMatrix() const { return _transformMatrix; }
    
    void BeginFrame();
    virtual void Frame(double time);
    void EndFrame();
    void Update();
    virtual void AfterUpdate();
    virtual void LastUpdate();
    
    
    virtual compareFunc GetCompareFunc();
    
    double GetSceneLocationTime() const {return _sceneLocationTime;}
    virtual void HasBeenLocatedToScene();
    virtual void HasBeenRemovedFromScene();
    
    virtual void Draw(Shader* shader);
    
    void SetIsOnScreen(bool isOnScreen);
    
    void SetGlobalPosition(GLKMatrix4 globalPosition, void * aditionalData, PivotObject *parent, bool afterUpdate);
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual GLKVector3 GetPosition() const { return GLKMatrix4GetTranslation(_transformMatrix); };
    virtual void SetPosition(const GLKVector3& position);
    
};

#endif /* defined(__TerminalExtraction__PivotObject__) */
