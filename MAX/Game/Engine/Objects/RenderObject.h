//
//  RenderObject.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/1/12.
//
//

#ifndef __TerminalExtraction__RenderObject__
#define __TerminalExtraction__RenderObject__

#include <iostream>

class Material;

using namespace std;

class RenderObject {
public:
    string          pictureTehnique;
    string          shadowTehnique;
    
    bool            isAnimated;
    bool            isShadowReceiver;
    bool            isShadowCaster;
    
    bool            isVisible;
    bool            isTransparent;
    bool            isSelfIllumination;
    
    float           additionalOpacity;
    
    
    RenderObject();
    virtual ~RenderObject();
    
    virtual void Render(int lod, Material *material) = 0;
};



#endif /* defined(__TerminalExtraction__RenderObject__) */
