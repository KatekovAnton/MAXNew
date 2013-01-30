//
//  MAXMapObject.h
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

class MAXMapObject : public PivotObject {
    
public:
    
    float mapW;
    float mapH;
    float mapTexW;
    float mapTexH;
    
    
    MapRenderObject* _renderObject;
    MAXMapMaterial* _material;
    
    MAXMapObject(shared_ptr<MAXContentMap> map);
    ~MAXMapObject();
    
    virtual RenderObject * GetRenderAspect();
    virtual Material * GetMaterial();
    
    virtual void Frame(double time);
};

#endif /* defined(__MAX__MapObject__) */
