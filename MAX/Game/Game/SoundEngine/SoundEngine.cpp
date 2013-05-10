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

void PlayEndOfTurnSound()
{}

void PlayButtonSound()
{}

void PlayEnemyDetectedSound()
{}