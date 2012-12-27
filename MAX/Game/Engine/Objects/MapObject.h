//
//  MapObject.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MapObject__
#define __MAX__MapObject__

#include <iostream>
#include "PivotObject.h"

class MAXContentMap;
class MapRenderObject;
class MAXMapMaterial;

class MapObject : public PivotObject {
    
public:
    
    float mapW;
    float mapH;
    float mapTexW;
    float mapTexH;
    
    
    MapRenderObject* _renderObject;
    MAXMapMaterial* _material;
    
    MapObject(shared_ptr<MAXContentMap> map);
    ~MapObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual void Frame(double time);
};

#endif /* defined(__MAX__MapObject__) */
