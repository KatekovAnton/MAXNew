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
#include "Texture.h"

using namespace std;

const int pal_size = 0x300;
const int max_width = 640;
const int max_height = 480;

MAXContentLoader* _sharedContentLoader = nullptr;

void MAXContentLoader::palshiftu(Color* pal, int s, int e)
{
    Color cl = pal[e];
    for (int i = e; i >= s + 1; i--)
        pal[i] = pal[i - 1];
    pal[s] = cl;
}

void MAXContentLoader::palshiftd(Color* pal, int s, int e)
{
    Color cl = pal[s];
    for (int i = s; i <= e - 1; i++)
        pal[i] = pal[i + 1];
    pal[e] = cl;
}

void MAXContentLoader::animatePalette(Color* thepal)
{
    palshiftd(thepal, 9, 12);//3
    palshiftu(thepal, 13, 16);//3
    palshiftu(thepal, 17, 20);//3
    palshiftu(thepal, 21, 24);//3
    
    palshiftu(thepal, 25, 30);//5
    //palblnkd(thepal, 31, 1 - frac(gct), gclgreen);
    
    palshiftu(thepal, 96, 102);//6
    palshiftu(thepal, 103, 109);//6
    palshiftu(thepal, 110, 116);//6
    palshiftu(thepal, 117, 122);//5
    palshiftu(thepal, 123, 127);//4
}

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

vector<Texture*> MAXContentLoader::CreatePalletes(Color* palette)
{
    Color* colors = new Color[pal_size];
    memcpy(colors, palette, 4 * pal_size);
    
    vector<Texture*> result;
    for(int i = 0;i<30;i++)
    {
        Color* currentPalette = (Color*)malloc(4 * pal_size);
        memcpy(currentPalette, colors, 4 * pal_size);
        result.push_back(new Texture(GL_LINEAR, (GLubyte*)currentPalette, pal_size, 1));
        if (i != 29)
            animatePalette(colors);
    }
    
    delete colors;
    return result;
}








