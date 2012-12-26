//
//  MAXContentMap.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXContentMap.h"
#include "BinaryReader.h"
#include "Sys.h"

MAXContentMap::MAXContentMap()
{}

MAXContentMap::~MAXContentMap()
{}

void MAXContentMap::LoadFromStream(BinaryReader *br)
{
    {
        char namebuffer[5]; 
        br->ReadBuffer(5, namebuffer);
        name = string(namebuffer);
        SysLogInfo("Map internal name:       %s", name.c_str());
    }
}