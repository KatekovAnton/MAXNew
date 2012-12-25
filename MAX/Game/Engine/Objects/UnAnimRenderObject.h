//
//  UnAnimRenderObject.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#ifndef __TerminalExtraction__UnAnimRenderObject__
#define __TerminalExtraction__UnAnimRenderObject__

#include <iostream>
#include "RenderObject.h"

class EngineMesh;

class UnAnimRenderObject : public RenderObject {
    
    const EngineMesh          *_mesh;
    
public:
    
    UnAnimRenderObject(const EngineMesh *mesh);
    ~UnAnimRenderObject();
    
    virtual void Render(int lod, Material *material);
};

#endif /* defined(__TerminalExtraction__UnAnimRenderObject__) */
