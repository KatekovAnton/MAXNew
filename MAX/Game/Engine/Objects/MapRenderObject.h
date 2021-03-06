//
//  MapRenderObject.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MapRenderObject__
#define __MAX__MapRenderObject__

#include <iostream>
#include "RenderObject.h"

using namespace std;

class MAXContentMap;
class EngineMesh;

class MapRenderObject : public RenderObject {
    
public:
    shared_ptr<EngineMesh> _mesh;
    
    MapRenderObject(shared_ptr<MAXContentMap> map);
    ~MapRenderObject();
    
    virtual void Render(int lod, Material *material);
};

#endif /* defined(__MAX__MapRenderObject__) */
