//
//  MyRandom.h
//  3DTest
//
//  Created by Katekov Anton on 15.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//

#ifndef _MyRandom_h
#define _MyRandom_h

#define ARC4RANDOM_MAX      0x100000000

static int nextInt()
{
    return rand();
}

static int nextIntMax(int max)
{
    return rand()%max;
}

static int nextIntMinMax(int min, int max)
{
    int range = max - min;
    int value = nextIntMax(range);
    return value + min;
}



static float nextFloat()
{
    return ((float)rand() / ARC4RANDOM_MAX);
}

static float nextFloatMax(float max)
{
    return nextFloat() * max;
}

static float nextFloatMinMax(float min, float max)
{
    float range = max - min;
    float value = nextFloatMax(range);
    return value + min;
}



static double nextDouble()
{
    return ((double)rand() / ARC4RANDOM_MAX);
}

static double nextDoubleMax(double max)
{
    return (nextDouble() * max);
}

static double nextDoubleMaxMin(double min, double max)
{
    double range = max - min;
    double value = nextDoubleMax(range);
    return value + min;
}

#endif