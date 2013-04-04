//
//  PFWave.cpp
//  MAX
//
//  Created by Alexey Malyshko on 3/27/13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "PFWave.h"

PFWave::PFWave(void)
{
    Clear();
}

PFWave::~PFWave(void)
{
    for (int i = 0; i < count; i++)
    {
        PFWaveCell* d = data[i];
        delete d;
    }
}

PFWaveCell* PFWave::GetItem()
{
    return data[pos];
}

void PFWave::Add(const int newX, const int newY, const unsigned int newCost, const unsigned char newDirection)
{
    PFWaveCell* d = NULL;
    if (count >= data.size())
    {
        d = new PFWaveCell(newX, newY, newCost, newDirection);
        data.push_back(d);
    }
    else
    {
        d = data[count];
        d->SetData(newX, newY, newCost, newDirection);
    }
    if (newCost < minCost)
    {
        minCost = newCost;
    }
    count++;
}

void PFWave::Add(CellPoint newPos, const unsigned int newCost, const unsigned char newDirection)
{
    Add(newPos.x, newPos.y, newCost, newDirection);
}

void PFWave::Add(PFWaveCell* wave)
{
    Add(wave->x, wave->y, wave->cost, wave->direction);
}

void PFWave::Clear(void)
{
    pos = 0;
    count = 0;
    minCost = INT_MAX;
}

bool PFWave::Start(void)
{
	pos = 0;
	return (count > 0);
}

bool PFWave::Next(void)
{
	pos++;
	return (pos < count);
}

//
//
//#include "PFWave.h"
//
//PFWave::PFWave(void)
//{
//    data = new Utils::USimpleContainer<PFWaveCell*>(10);
//    Clear();
//}
//
//PFWave::~PFWave(void)
//{
//    for (int i = 0; i < count; i++)
//    {
//        PFWaveCell* d = data->objectAtIndex(i);
//        delete d;
//    }
//    delete data;
//}
//
//PFWaveCell* PFWave::GetItem()
//{
//    return data->objectAtIndex(pos);
//}
//
//void PFWave::Add(const int newX, const int newY, const unsigned int newCost, const unsigned char newDirection)
//{
//    if (count >= data->GetCount())
//        data->addObject(new PFWaveCell(newX, newY, newCost, newDirection));
//    
//    else
//        data->objectAtIndex(count)->SetData(newX, newY, newCost, newDirection);
//    
//    if (newCost < minCost)
//        minCost = newCost;
//    
//    count++;
//}
//
//void PFWave::Add(CellPoint newPos, const unsigned int newCost, const unsigned char newDirection)
//{
//    Add(newPos.x, newPos.y, newCost, newDirection);
//}
//
//void PFWave::Add(PFWaveCell* wave)
//{
//    Add(wave->x, wave->y, wave->cost, wave->direction);
//}
//
//void PFWave::Clear(void)
//{
//    pos = 0;
//    count = 0;
//    minCost = INT_MAX;
//}
//
//bool PFWave::Start(void)
//{
//	pos = 0;
//	return (count > 0);
//}
//
//bool PFWave::Next(void)
//{
//	pos++;
//	return (pos < count);
//}
//
