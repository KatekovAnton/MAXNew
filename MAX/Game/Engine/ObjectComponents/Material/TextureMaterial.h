//
//  TextureMaterial.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#ifndef __TerminalExtraction__TextureMaterial__
#define __TerminalExtraction__TextureMaterial__

#include <iostream>
#include "Material.h"

using namespace std;

class Texture;
class Shader;

class TextureMaterial : public Material {
    Texture     *_texture;

public:
    
    TextureMaterial();
    TextureMaterial(string name);
    ~TextureMaterial();
    
    void ApplyLod(int lod, Shader *shader);
    
    void SetTexture(Texture* texture);
    Texture *GetTexture() const { return _texture; };
    
    virtual void SetFrame(int frame);
    virtual void StopBackgroundLoading();
};

#endif /* defined(__TerminalExtraction__TextureMaterial__) */
