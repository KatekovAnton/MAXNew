//
//  SoundEngine.h
//  MAX
//
//  Created by Anton Katekov on 08.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__SoundEngine__
#define __MAX__SoundEngine__

#include <iostream>
#include "miniPrefix.h"

struct _SoundElement
{
    string _name;
    int _id;
    float _length;
    double _startTime;
};
typedef _SoundElement SoundElement;

class SoundEngine {
    
    map<string, SoundElement> playedSound;
    
public:
    
    static SoundEngine* sharedInstance();
    
    void PlayEndOfTurnSound();
    void PlayButtonSound();
    void PlayEnemyDetectedSound();
    
};

#endif /* defined(__MAX__SoundEngine__) */
