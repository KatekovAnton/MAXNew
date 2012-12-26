//
//  MAXContetnLoader.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXContetnLoader.h"
#include "MAXContentMap.h"
#include "BinaryReader.h"
#include "FileManger.h"
#include <string>

using namespace std;

MAXContentLoader* _sharedContentLoader = nullptr;

MAXContentLoader::MAXContentLoader()
{}

shared_ptr<MAXContentMap> MAXContentLoader::LoadMapWithName(string name)
{
    BinaryReader* br = new BinaryReader(name);
    
    shared_ptr<MAXContentMap> result = shared_ptr<MAXContentMap>(new MAXContentMap());
    result->LoadFromStream(br);
    
    delete br;
    
    return result;
}


MAXContentLoader* MAXContentLoader::SharedLoader()
{
    if (!_sharedContentLoader)
        _sharedContentLoader = new MAXContentLoader();
    return _sharedContentLoader;
}