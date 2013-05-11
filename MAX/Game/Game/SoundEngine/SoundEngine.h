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

class SoundEngineDelegate;

struct _SoundElement
{
    string _name;
    int _id;
    float _length;
    double _startTime;
    
    SoundEngineDelegate *_delegate_w;
};
typedef _SoundElement SoundElement;

class SoundEngine {
    
    
    map<SOUND_TYPE, vector<string>> _systemSoundNames;
    vector<SoundElement>            _playedSound;
    
public:
    
    SoundEngine();
    ~SoundEngine();
    
    static SoundEngine* sharedInstance();
    
    void PlaySystemSound(SOUND_TYPE type);
    
    int PlayGameSound(string fileName);
    void StopGameSound(int sound);
    
    void CheckStoppedSound();
    
};

#endif /* defined(__MAX__SoundEngine__) */
