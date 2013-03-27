//
//  PFWave.h
//  MAX
//
//  Created by Alexey Malyshko on 3/27/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__PFWave__
#define __MAX__PFWave__

#include <iostream>
#include "PFWaveCell.h"
#include "cocos2d.h"

using namespace std;

class PFWaveCell;

class PFWave
{
	int                 pos;
	int                 count;
    vector<PFWaveCell*> data;
public:
	int                 minCost;

    PFWave();
    ~PFWave();
    
    PFWaveCell* GetItem();
    void Add(const int newX, const int newY, const unsigned int newCost, const unsigned char newDirection);
    void Add(CellPoint newPos, const unsigned int newCost, const unsigned char newDirection);
    void Add(PFWaveCell* wave);
    void Clear(void);
    bool Start(void);
    bool Next(void);
};

#endif /* defined(__MAX__PFWave__) */
