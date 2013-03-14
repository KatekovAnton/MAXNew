//
//  MAXContentMap.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXContentMap__
#define __MAX__MAXContentMap__

#include <iostream>
#include "MAXContentConstants.h"

using namespace std;

class BinaryReader;

class MAXContentMap {
    
public:
    
    string name;
    int w, h;
    short elementCount;
    
    short* map;
    char* groundType;//0-ground 1-water 2-coast 3-unpassable
    
    //пиксели тайлов
    char* mapElements;
    char* minimap;
    Color* palette;
    
    MAXContentMap();
    ~MAXContentMap();
    
    void LoadFromStream(BinaryReader* br);
};

#endif /* defined(__MAX__MAXContentMap__) */
