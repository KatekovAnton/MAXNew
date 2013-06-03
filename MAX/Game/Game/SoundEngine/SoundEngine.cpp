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
#include "MyRandom.h"

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
{
    _holdEffects = false;
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.4);
    {
        vector<string> names;
        names.push_back("f001.wav");
        names.push_back("f004.wav");
        names.push_back("f005.wav");
        names.push_back("f006.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_READY, names));
    }
    {
        vector<string> names;
        names.push_back("f053.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_START_OF_TURN, names));
    }
    {
        vector<string> names;
        names.push_back("mengens3.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_BUTTON_LARGE, names));
    }
    {
        vector<string> names;
        names.push_back("menu38.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_BUTTON_AVERAGE, names));
    }
    {
        vector<string> names;
        names.push_back("mengens4.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_BUTTON_SMALL, names));
    }
    {
        vector<string> names;
        names.push_back("f070.wav");
        names.push_back("f071.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_ENEMY_DETECTED, names));
    }
    {
        vector<string> names;
        names.push_back("f201.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_SUBMARINE_DETECTED, names));
    }
    {
        vector<string> names;
        names.push_back("f284.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_BUILDING, names));
    }
    //button buy upg kbuy0.wav
    //load unit to airtrans load.wav
    //load/unlod/create scale.wav
}

SoundEngine::~SoundEngine()
{}

void SoundEngine::PlaySystemSound(SOUND_TYPE type)
{
    if (_systemSoundNames.count(type) == 0) 
        return;
    
    for (int i = 0; i < _playedSound.size(); i++) 
        if (_playedSound[i]._type == type) 
            return;
        
    vector<string> soundNames = _systemSoundNames[type];
    string name = soundNames[nextIntMax(soundNames.size())];
    SoundElement element;
    element._type = type;
    element._looped = false;
    element._id = SimpleAudioEngine::sharedEngine()->playEffect(name.c_str(), false, 0.6);
    element._length = SimpleAudioEngine::sharedEngine()->lengthOfEffect(name.c_str());
    element._delegate_w = NULL;
    element._startTime = engine->FullTime();
    _playedSound.push_back(element);
}

void SoundEngine::PlayExplodeSound(EXPLODE_SOUND_TYPE type)
{}

int SoundEngine::PlayGameSound(string fileName, SoundEngineDelegate* delegate, bool looped, float volume)
{
    if (_holdEffects) {
        return -1;
    }
    SoundElement element;
    element._type = SOUND_TYPE_NONE;
    element._looped = looped;
    element._id = SimpleAudioEngine::sharedEngine()->playEffect(fileName.c_str(), looped, volume);
    element._length = 0;
    element._delegate_w = delegate;
    element._startTime = engine->FullTime();
    _playedSound.push_back(element);
    return element._id;
}

void SoundEngine::StopGameSound(int sound)
{
    SimpleAudioEngine::sharedEngine()->stopEffect(sound);
    std::vector<SoundElement>::iterator element;
    element = _playedSound.begin();
    for (; element != _playedSound.end(); element++)
    {
        SoundElement *item = &(*element);
        if (item->_id == sound && !item->_looped)
        {
            if (item->_delegate_w)
                item->_delegate_w->SoundDidFinishPlaying(item->_id);
            
            _playedSound.erase(element);
            break;
        }
    }
}

void SoundEngine::CheckStoppedSound()
{
    std::vector<SoundElement>::iterator element;
    bool finded = true;
    while (finded)
    {
        finded = false;
        element = _playedSound.begin();
        for (; element != _playedSound.end(); element++)
        {
            SoundElement *item = &(*element);
            double fifnishTime = item->_startTime + item->_length;
            double currentTime = engine->FullTime();
            if (fifnishTime < currentTime)
            {
                finded = true;
                if (item->_delegate_w)
                    item->_delegate_w->SoundDidFinishPlaying(item->_id);
                
                _playedSound.erase(element);
                break;
            }
        }
    }
}

void SoundEngine::SetHoldEffects(bool hold)
{
    _holdEffects = hold;
}
