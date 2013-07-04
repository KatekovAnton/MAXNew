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

#define SOUND SoundEngine::sharedInstance()

class SoundEngineDelegate;

struct _SoundElement
{
    int     _id;
    double  _length;
    double  _startTime;
    bool    _looped;
    
    SoundEngineDelegate *_delegate_w;
    
    SOUND_TYPE _type;
};
typedef _SoundElement SoundElement;

class SoundEngine {
    
	//key means blocked sound by array of another sounds
	map<SOUND_TYPE, vector<SOUND_TYPE>> _systemSoundBlocks;
    

    map<SOUND_TYPE, vector<string>> _systemSoundNames;
    map<EXPLODE_SOUND_TYPE, vector<string>> _explodeSoundNames;
    vector<SoundElement>            _playedSound;
    
    bool _holdEffects;
    
public:
    
    SoundEngine();
    ~SoundEngine();
    
    static SoundEngine* sharedInstance();
    
    void PlaySystemSound(SOUND_TYPE type);
    void PlayExplodeSound(EXPLODE_SOUND_TYPE type);
    
    int PlayGameSound(string fileName, SoundEngineDelegate* delegate, bool looped, float volume);
    void StopGameSound(int sound);
    
    void CheckStoppedSound();
    
    void SetHoldEffects(bool hold);
    
};

#endif /* defined(__MAX__SoundEngine__) */
