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
class MAXUnitMaterial;
class BinaryReader;
class Texture;
class EngineMesh;

class MAXUnitObject;

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
    void** loadedData;
    
public:
    
    shared_ptr<EngineMesh> unitMesh;
    Texture* defaultPalette;
    
    static MAXContentLoader* SharedLoader();
    
    MAXContentLoader();
    ~MAXContentLoader();
    
    shared_ptr<MAXContentMap> LoadMapWithName(string name);
    vector<Texture*> CreatePalletes(Color* palette);
    Texture* TextureFromIndexAndPalette(int w, int h, unsigned char* indexes, unsigned char* palette);
    Texture* TextureFromIndexAndDefaultPalette(int w, int h, unsigned char* indexes);
    int FindImage(string name);
    void LoadFrame(BinaryReader* source, int index, MAXUnitMaterial* target, long baseOffset);
    MAXUnitMaterial* LoadUnitMaterial(string name);
    
#pragma mark - fabric
    shared_ptr<MAXUnitObject> CreateUnit(string bodyName);
};

#endif /* defined(__MAX__MAXContetnLoader__) */
