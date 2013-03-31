//
//  PFWaveCell.cpp
//  MAX
//
//  Created by Alexey Malyshko on 3/27/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "PFWaveCell.h"

CellPoint CellPointMake(const int _x, const int _y)
{
    CellPoint result;
    result.x = _x;
    result.y = _y;
    return result;
}

PFWaveCell::PFWaveCell(const int _x, const int _y, const unsigned char _cost, const unsigned char _direction)
{
    x = _x;
    y = _y;
    cost = _cost;
    direction = _direction;
}

void PFWaveCell::SetData(const int _x, const int _y, const unsigned char _cost, const unsigned char _direction)
{
    x = _x;
    y = _y;
    cost = _cost;
    direction = _direction;
}

CellPoint PFWaveCell::GetPosForDirection(const unsigned char _direction)
{
	return CellPointMake(x + DirToDX(_direction), y + DirToDY(_direction));
}

CellPoint PFWaveCell::GetPosFrom(void)
{
    return CellPointMake(x - DirToDX(direction), y - DirToDY(direction));
}

int PFWaveCell::DirToDX(const unsigned char _direction)
{
	int result = -1;
	if ((_direction == 0) || (_direction == 4))
	{
		result = 0;
	}
	else if ((_direction == 1) || (_direction == 2) || (_direction == 3))
	{
		result = 1;
	}
	return result;
}

int PFWaveCell::DirToDY(const unsigned char _direction)
{
	int result = -1;
	if ((_direction == 2) || (_direction == 6))
	{
		result = 0;
	}
	else if ((_direction == 3) || (_direction == 4) || (_direction == 5))
	{
		result = 1;
	}
	return result;
}
