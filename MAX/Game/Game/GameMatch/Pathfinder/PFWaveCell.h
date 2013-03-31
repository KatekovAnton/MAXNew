//
//  PFWaveCell.h
//  MAX
//
//  Created by Alexey Malyshko on 3/27/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__PFWaveCell__
#define __MAX__PFWaveCell__

#include <iostream>

typedef struct 
{
    int x;
    int y;
} CellPoint;

CellPoint CellPointMake(const int _x, const int _y);

class PFWaveCell
{
public:
	int x, y;                   // destination coordinates
	unsigned char cost;			// remain cost to this point
	unsigned char direction;	// direction to point
    
    PFWaveCell(const int _x, const int _y, const unsigned char _cost, const unsigned char _direction);

    void SetData(const int _x, const int _y, const unsigned char _cost, const unsigned char _direction);

    CellPoint GetPosForDirection(const unsigned char _direction);
    CellPoint GetPosFrom(void);
    static int DirToDX(const unsigned char _direction);
    static int DirToDY(const unsigned char _direction);
};

#endif /* defined(__MAX__PFWaveCell__) */
