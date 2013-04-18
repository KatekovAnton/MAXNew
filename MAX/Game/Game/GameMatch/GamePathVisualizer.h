//
//  GamePathVisualizer.h
//  MAX
//
//  Created by Anton Katekov on 18.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__GamePathVisualizer__
#define __MAX__GamePathVisualizer__

#include <iostream>
#include <vector>

using namespace std;

struct __pathElement
{
    int image; //0 to 15
    int x;
    int y;
    int unitLevel;
};

typedef __pathElement PathElement;

class GameEffect;

class GamePathVisualizer {
    
    vector<PathElement> _elements;
    vector<GameEffect*> _views;
    
    GameEffect* CreateEffectWithImage(int imageType);
    
public:
    
    GamePathVisualizer();
    ~GamePathVisualizer();
    
    void VisualizePath(vector<PathElement> elements);
    void Clear();
    void RemoveFirstItem();
};

#endif /* defined(__MAX__GamePathVisualizer__) */
