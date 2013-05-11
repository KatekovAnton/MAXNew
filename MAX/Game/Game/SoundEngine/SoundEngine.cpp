//
//  SoundEngine.cpp
//  MAX
//
//  Created by Anton Katekov on 08.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "SoundEngine.h"
#include "MAXEngine.h"
#include "SimpleAudioEngine.h"
#include "SoundEngineDelegate.h"

#define SOUND_ENEMY_DETECTED 0
#define SOUND_END_OF_TURN 1

using namespace CocosDenshion;

static SoundEngine* _sharedSoundengineInstance = NULL;

SoundEngine* SoundEngine::sharedInstance()
{
    if (!_sharedSoundengineInstance) {
        _sharedSoundengineInstance = new SoundEngine();
    }
    return _sharedSoundengineInstance;
}

SoundEngine::SoundEngine()
{}

SoundEngine::~SoundEngine()
{}

void SoundEngine::PlaySystemSound(SOUND_TYPE type)
{}

int SoundEngine::PlayGameSound(string fileName, SoundEngineDelegate* delegate)
{
    return 0;
}

void SoundEngine::StopGameSound(int sound)
{
    
}

void SoundEngine::CheckStoppedSound()
{
    std::vector<SoundElement>::iterator element;
    bool finded = true;
    while (finded)
    {
        finded = false;
        element = _playedSound.begin();
        for (; element != _playedSound.end(); element++) {
            SoundElement *item = &(*element);
            if (item->_startTime + item->_length > engine->FullTime()) {
                finded = true;
                if (item->_delegate_w) {
                    item->_delegate_w->SoundDidFinishPlaying(item->_id);
                }
                _playedSound.erase(element);
                break;
            }
        }
    }
}


