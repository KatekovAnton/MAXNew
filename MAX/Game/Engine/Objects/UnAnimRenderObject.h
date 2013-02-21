//
//  UnAnimRenderObject.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/3/12.
//
//

#ifndef __TerminalExtraction__UnAnimRenderObject__
#define __TerminalExtraction__UnAnimRenderObject__

#include <iostream>
#include "RenderObject.h"

class EngineMesh;

class UnAnimRenderObject : public RenderObject {
    
    shared_ptr<EngineMesh>          _mesh;
    Material* _material;
public:
    
    UnAnimRenderObject(shared_ptr<EngineMesh> mesh);
    ~UnAnimRenderObject();
    
    virtual void Render(int lod, Material *material);
};

#endif /* defined(__TerminalExtraction__UnAnimRenderObject__) */
