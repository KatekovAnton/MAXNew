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

const int pal_size = 0x300;
const int max_width = 640;
const int max_height = 480;

MAXContentMap::MAXContentMap()
{}

MAXContentMap::~MAXContentMap()
{
    delete []minimap;
    delete []map;
    delete []mapElements;
    delete []palette;
    delete []groundType;
}

void MAXContentMap::LoadFromStream(BinaryReader *br)
{
    {//internal name
        char namebuffer[5]; 
        br->ReadBuffer(5, namebuffer);
        name = string(namebuffer);
    }
    
    {//size
        w = br->ReadInt16();
        h = br->ReadInt16();
    }
    
    {//minimap
        minimap = new char[w * h];
        br->ReadBuffer(w*h, minimap);
    }
    
    {//tile indics
        map = new short[w * h];
        br->ReadBuffer(w * h * 2, (char*)map);
    }
    
    {//tiles
        elementCount = br->ReadInt16();
        mapElements = new char[elementCount * 64 * 64];
        br->ReadBuffer(elementCount * 64 * 64, mapElements);
    }
    
    {//palette
        palette = new Color[pal_size/3];
        for (int i = 0; i < pal_size/3; i++)
        {
            palette[i].r = br->ReadChar();
            palette[i].g = br->ReadChar();
            palette[i].b = br->ReadChar();
            palette[i].a = 1.0;
        }
    }
    
    {//ground types
        groundType = new char[elementCount];
        br->ReadBuffer(elementCount, groundType);
    }
}

