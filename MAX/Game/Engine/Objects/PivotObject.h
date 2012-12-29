//
//  PivotObject.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/31/12.
//
//

#ifndef __TerminalExtraction__PivotObject__
#define __TerminalExtraction__PivotObject__

#include <iostream>
#include "Geometry.h"

using namespace std;

class EditorData;
class ObjectBehaviourModel;
class RenderMatrixController;
class RenderObject;
class Material;
class Shader;

class PivotObject {
protected:
    GLKMatrix4              _renderMatrix;
    GLKMatrix4              _transformMatrix;
    
    bool                _needMouseCast;
    bool                _isOnScreen;

    unsigned _objectId;
    
public:

    unsigned GetObjectId() const {return _objectId; };
    void SetObjectId(unsigned newValue) { _objectId = newValue; };
    
    bool        moved;
    bool        forceHidden;
    
    PivotObject();
    virtual ~PivotObject();

    GLKMatrix4 GetRenderMatrix();
    GLKMatrix4 GetTransformMatrix();
    
    void Update();
    virtual void AfterUpdate();
    GLKMatrix4 CalculateRenderMatrix(GLKMatrix4 transform);
    virtual void Frame(double time);
    void BeginFrame();
    void EndFrame();
    
    virtual void Draw(Shader* shader);
    
    void SetIsOnScreen(bool isOnScreen);
    
    void SetGlobalPosition(GLKMatrix4 globalPosition, void * aditionalData, PivotObject *parent, bool afterUpdate);
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual GLKVector3 GetPosition();
    virtual void SetPosition(const GLKVector3& position);
    
};

#endif /* defined(__TerminalExtraction__PivotObject__) */
