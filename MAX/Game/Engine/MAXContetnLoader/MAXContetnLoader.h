//
//  MAXContetnLoader.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXContetnLoader__
#define __MAX__MAXContetnLoader__

#include <iostream>
#include "MAXContetnConstants.h"

#define MAXSCL MAXContentLoader::SharedLoader()

using namespace std;

class MAXContentMap;
class Texture;

class MAXContentLoader {
    
    void palshiftu(Color* pal, int s, int e);
    void palshiftd(Color* pal, int s, int e);
    void animatePalette(Color* thepal);
public:
    MAXContentLoader();
    
    shared_ptr<MAXContentMap> LoadMapWithName(string name);
    
    
    static MAXContentLoader* SharedLoader();
    
    Texture* CreatePalletes(Color* palette);
};

#endif /* defined(__MAX__MAXContetnLoader__) */
