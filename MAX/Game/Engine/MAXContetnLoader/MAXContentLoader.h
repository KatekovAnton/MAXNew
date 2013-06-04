//
//  MAXContentLoader.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXContentLoader__
#define __MAX__MAXContentLoader__

#include <iostream>
#include <vector>
#include <map>
#include "miniPrefix.h"

#define MAXSCL MAXContentLoader::SharedLoader()

using namespace std;

class MAXContentMap;
class MAXUnitMaterial;
class BinaryReader;
class Texture;
class EngineMesh;

class MAXUnitObject;
class MAXObjectConfig;
class MAXEffectObject;

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

struct TileInformation
{
    float topX;
    float topY;
    float bottomX;
    float bottomY;
};

class MAXContentLoader {
    
    void palshiftu(Color* pal, int s, int e);
    void palshiftd(Color* pal, int s, int e);
    void animatePalette(Color* thepal);
    void animateUnitPalette(Color* thepal);
    
    typdiritem *dir;
    typhdr hdr;
    BinaryReader* inf;
    void** loadedData;
    
public:
    
    map<string, MAXUnitMaterial*> externalMaterials;
    
    shared_ptr<EngineMesh> unitMesh;
    Texture* defaultPalette;
    Texture* resourceTiles;
    
    static MAXContentLoader* SharedLoader();
    
    MAXContentLoader();
    ~MAXContentLoader();
    
    shared_ptr<MAXContentMap> LoadMapWithName(string name);
    void SetMapColorsToDefaultPalette(Color* thepal);
    vector<Texture*> CreatePalletes(Color* palette);
    vector<Texture*> CreateUnitPalletes(Color* palette);
    Texture* TextureIdexedFromIndex(int w, int h, unsigned char* indexes);
    Texture* TextureIdexedFromColors(int w, int h, Color* colors);
    Texture* TextureFromIndexAndPalette(int w, int h, unsigned char* indexes, unsigned char* palette);
    Texture* TextureFromIndexAndDefaultPalette(int w, int h, unsigned char* indexes);
    Texture* TextureForResourceRenderer();
    vector<Texture*> TexturePalletesFormDefaultPalleteAndPlayerColor(const Color& color);
    int FindImage(string name);
    void LoadUnitFrame(BinaryReader* source, int index, MAXUnitMaterial* target, long baseOffset);
    void LoadUnitShadow(BinaryReader* shadowSource, int index, MAXUnitMaterial* target, long shadowBaseOffset);

    MAXUnitMaterial* LoadUnitMaterial(string name, string shadowName);
    MAXUnitMaterial* LoadEffectMaterial(string name);
    MAXUnitMaterial* LoadEffectMaterial(vector<string> names);
    MAXUnitMaterial* LoadEffectMaterialfromSingleImage(string name);
    MAXUnitMaterial* LoadEffectMaterialfromExternalImage(string name);
    MAXRESTextureData CreateTexture2Data(string name);
    
#pragma mark - memory
    void ClearImageCache();
    
#pragma mark - creation
    
    MAXEffectObject* CreateEffect(MAXObjectConfig* effectConfig, float size, bool animated, vector<string> imageNames);
    MAXEffectObject* CreateEffect(MAXObjectConfig* effectConfig, float size, bool animated);
    MAXEffectObject* CreateSingleEffect(MAXObjectConfig* effectConfig, float size);
    MAXUnitObject* CreateUnit(MAXObjectConfig* unitConfig);
    cocos2d::CCTexture2D* CreateTexture2DFromSimpleImage(string name);
    cocos2d::CCTexture2D* CreateTexture2DFromSimpleImage(string name, Color transparent);
    cocos2d::CCTexture2D* CreateTexture2DFromPalettedImage(string name);
    
    cocos2d::CCSprite* CreateSpriteFromSimpleImage(string name);
    cocos2d::CCSprite* CreateSpriteFromSimpleImage(string name, Color transparent);
};

#endif /* defined(__MAX__MAXContentLoader__) */
