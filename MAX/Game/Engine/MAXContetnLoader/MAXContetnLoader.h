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
#include <vector>
#include "MAXContetnConstants.h"

#define MAXSCL MAXContentLoader::SharedLoader()

using namespace std;

class MAXContentMap;
class BinaryReader;
class Texture;


struct typhdr
{
    char _id[4];
    int diroffset;
    int dirlength;
};

struct typdiritem
{
    char name[8];
    int offset;
    int size;
};

class MAXContentLoader {
    
    void palshiftu(Color* pal, int s, int e);
    void palshiftd(Color* pal, int s, int e);
    void animatePalette(Color* thepal);
    
    
    typdiritem *dir;
    typhdr hdr;
    BinaryReader* inf;
    
public:
    MAXContentLoader();
    ~MAXContentLoader();
    
    shared_ptr<MAXContentMap> LoadMapWithName(string name);
    
    
    static MAXContentLoader* SharedLoader();
    
    vector<Texture*> CreatePalletes(Color* palette);
    shared_ptr<Texture> TextureFromIndexAndPalette(int w, int h, unsigned char* indexes, unsigned char* palette);
    shared_ptr<Texture> TextureFromIndexAndDefaultPalette(int w, int h, unsigned char* indexes);
};

#endif /* defined(__MAX__MAXContetnLoader__) */
