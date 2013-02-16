//
//  MAXMapMaterial.h
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXMapMaterial__
#define __MAX__MAXMapMaterial__

#include <iostream>
#include <vector>
#include "Material.h"

class Texture;
class MAXContentMap;

using namespace std;

class MAXMapMaterial : public Material
{
public:
    vector<Texture*> palettes;
    
    float texW;
    float texH;
    
    int index;
    double time;
    
    Texture* mapElementsSingle;
//    Texture* fullMapTexture;
    Texture* currentPalette;
    
    MAXMapMaterial(shared_ptr<MAXContentMap> map);
    ~MAXMapMaterial();
    
    void DoFrame(double elapsedTime);
    
    virtual void ApplyLod(int lod, Shader *shader);
    virtual void SetFrame(int frame);
    virtual void StopBackgroundLoading();
};

#endif /* defined(__MAX__MAXMapMaterial__) */
