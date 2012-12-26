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

class MAXContentLoader {
    
public:
    MAXContentLoader();
    
    shared_ptr<MAXContentMap> LoadMapWithName(string name);
    
    
    static MAXContentLoader* SharedLoader();
};

#endif /* defined(__MAX__MAXContetnLoader__) */
