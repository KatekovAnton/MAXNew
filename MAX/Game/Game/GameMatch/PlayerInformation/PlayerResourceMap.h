//
//  PlayerResourceMap.h
//  MAX
//
//  Created by  Developer on 30.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__PlayerResourceMap__
#define __MAX__PlayerResourceMap__

#include <iostream>

class PlayerResourceMap {
    
    int _mapW;
    int _mapH;
    
    
    int IndexOf(const int x, const int y) const { return  _mapW * y + x; };
    
public:
    
    unsigned char *_resScan;
    
    PlayerResourceMap(const int mapW, const int mapH);
    ~PlayerResourceMap();
    
    void AddCell(const int x, const int y);
    
};

#endif /* defined(__MAX__PlayerResourceMap__) */
