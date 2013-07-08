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
    {
        vector<string> names;
        names.push_back("f162.wav");
        names.push_back("f165.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_CONSTRUCTION_COMPLETE, names));
    }
    {
        vector<string> names;
        names.push_back("f166.wav");
        names.push_back("f169.wav");
        names.push_back("f216.wav");
        names.push_back("f217.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_UNIT_COMPLETED, names));
    }
    {
        vector<string> names;
        names.push_back("f050.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_MOVE_TAPE_TO_BULD, names));
    }
    {
        vector<string> names;
        names.push_back("f085.wav");
        names.push_back("f089.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_RELOADED, names));
    }
    {
        vector<string> names;
        names.push_back("f138.wav");
        names.push_back("f270.wav");
        names.push_back("f271.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_AMMO_LOW, names));
    }
    {
        vector<string> names;
        names.push_back("f142.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_AMMO_NONE, names));
    }
    {
        vector<string> names;
        names.push_back("f145.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_MOVEMENT_EXHAUSTED, names));
    }
    {
        vector<string> names;
        names.push_back("f150.wav");
        names.push_back("f151.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_STATUS_YELLOW, names));
    }
    {
        vector<string> names;
        names.push_back("f154.wav");
        names.push_back("f155.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_STATUS_RED, names));
    }
    {
        vector<string> names;
        names.push_back("f210.wav");
        names.push_back("f211.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_UNITS_REPAIRED, names));
    }
    {
        vector<string> names;
        names.push_back("f219.wav");
        names.push_back("f220.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_UNIT_REPAIRED, names));
    }
    {
        vector<string> names;
        names.push_back("f219.wav");
        names.push_back("f220.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_AMMO_NONE, names));
    }
    {
        vector<string> names;
        names.push_back("f234.wav");
        names.push_back("f236.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_UNIT_DESTROYED, names));
    }
	{
        vector<string> names;
        names.push_back("f229.wav");
        names.push_back("f230.wav");
        names.push_back("f231.wav");
        names.push_back("f232.wav");
        names.push_back("f255.wav");
        names.push_back("f256.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_ENEMY_FIRING_ON_UNIT, names));
    }
    {
        vector<string> names;
        names.push_back("f250.wav");
        names.push_back("f251.wav");
        _systemSoundNames.insert(std::pair<SOUND_TYPE, vector<string>>(SOUND_TYPE_UNIT_FIRING, names));
    }
//    SOUND_TYPE_INFILTRATOR_FAIL,
//    SOUND_TYPE_GAME_SAVED,
//    SOUND_TYPE_FUEL_STORAGE_FULL,//f057.wav
//    SOUND_TYPE_RAWM_STORAGE_FULL,//f061.wav
//    SOUND_TYPE_GOLD_STORAGE_FULL,//f066.wav
//    SOUND_TYPE_RESEARCH_COMPLETE, //f093.wav
//    SOUND_TYPE_NO_PATH_TO_DESTINATION, //f094.wav
//    SOUND_TYPE_PATH_BLOCKED,//f095.wav
//    SOUND_TYPE_ATTEMPT_FAILED,//f007.wav
//    SOUND_TYPE_ON_SENTRY,//f158.wav
//    SOUND_TYPE_COMPLETE_AND_READY,//f164.wav
//    SOUND_TYPE_CLEARING_AREA,//f171.wav
//    SOUND_TYPE_STARTGAME_SELECT_SIDE,//f176.wav f177.wav
//    SOUND_TYPE_LAYING_MINES,//f181.wav f182.wav
//    SOUND_TYPE_REMOVING_MINES,// f186.wav f187.wav
//    SOUND_TYPE_SURVEYING,//f191.wav f192.wav
//    SOUND_TYPE_ATTACKING,//f196.wav f198.wav
//    SOUND_TYPE_NEW_UNITS_AVAILABLE_THIS_TURN,//f206.wav f207.wav f215.wav
//    SOUND_TYPE_TRANSFER_COMPLETED,//f224.wav
//    SOUND_TYPE_UNIT_CAPTURED,//f239.wav  f242.wav
//    SOUND_TYPE_UNIT_HAS_BEEN_SEASED,//f243.wav
//    SOUND_TYPE_UNIT_DISABLED,//f244.wav
//    SOUND_TYPE_UNIT_MUST_BE_REPAIRED,//f247.wav f265.wav
//    SOUND_TYPE_UNIT_NOT_FUNCTIONATING,//f249.wav
//    SOUND_TYPE_STARTGAME_SELECT_LAND_POSITION,//f278.wav
//    SOUND_TYPE_MISSION_SUCCESSFULL,//f283.wav
//    SOUND_TYPE_CANT_START,//mstat18.wav
    
    
    
    {
        vector<string> names;
        names.push_back("explmed.wav");
        names.push_back("explsmal.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_LAND_SMALL_UNIT, names));
    }
    {
        vector<string> names;
        names.push_back("bldexplg.wav");
        names.push_back("expllrge.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_LAND_SMALL_BUILD, names));
    }
    {
        vector<string> names;
        names.push_back("expllrge.wav");
        names.push_back("expbuld5.wav");
        names.push_back("expbuld6.wav");
        names.push_back("explbld1.wav");
        names.push_back("explbld2.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_LAND_LARGE_BUILD, names));
    }
    {
        vector<string> names;
        names.push_back("eplowet1.wav");
        names.push_back("eplowet2.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_SEA_SMALL_UNIT, names));
    }
    {
        vector<string> names;
        names.push_back("eplowet1.wav");
        names.push_back("eplowet2.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_SEA_SMALL_BUILD, names));
    }
    {
        vector<string> names;
        names.push_back("cbldexp1.wav");
        names.push_back("cbldexp2.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_SEA_LARGE_BUILD, names));
    }
    {
        vector<string> names;
        names.push_back("sub14.wav");
        names.push_back("sub16.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_UNDERWATER_UNIT, names));
    }
    {
        vector<string> names;
        names.push_back("cmine16.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_MINE, names));
    }
    {
        vector<string> names;
        names.push_back("expsdirt.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_DIRT, names));
    }
    {
        vector<string> names;
        names.push_back("explsmal.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_AIR, names));
    }
    {
        vector<string> names;
        names.push_back("infan15.wav");
        names.push_back("infan15.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_INFANTRY, names));
    }
    {
        vector<string> names;
        names.push_back("infil15.wav");
        names.push_back("infil15.wav");
        _explodeSoundNames.insert(std::pair<EXPLODE_SOUND_TYPE, vector<string>>(EXPLODE_SOUND_TYPE_INFILTRATOR, names));
    }

	//blocks
	
	{
		vector<SOUND_TYPE> items;
		items.push_back(SOUND_TYPE_ENEMY_FIRING_ON_UNIT);
		_systemSoundBlocks.insert(std::pair<SOUND_TYPE, vector<SOUND_TYPE>>(SOUND_TYPE_UNIT_DESTROYED, items));
	}
}

SoundEngine::~SoundEngine()
{}

void SoundEngine::PlaySystemSound(SOUND_TYPE type)
{
	if (_systemSoundNames.count(type) == 0) 
        return;
    
	CheckStoppedSound();
    for (int i = 0; i < _playedSound.size(); i++) 
        if (_playedSound[i]._type == type) 
            return;
        

	if (_systemSoundBlocks.count(type) != 0)
	{
		vector<SOUND_TYPE> blocks = _systemSoundBlocks[type];
		for (int i = 0; i < blocks.size(); i++)
		{
			SOUND_TYPE block = blocks[i];
			bool allstoped = false;
			while (!allstoped)
			{
				allstoped = true;
				for (int j = 0; j < _playedSound.size(); j++)
				{
					if (_playedSound[j]._type == block)
					{
						StopGameSound(_playedSound[j]._id);
						allstoped = false;
						break;
					}
				}
			}
		}

	}

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
{
    if (_explodeSoundNames.count(type) == 0)  
        return;
    
    vector<string> soundNames = _explodeSoundNames[type];
    string name = soundNames[nextIntMax(soundNames.size())];
    SimpleAudioEngine::sharedEngine()->playEffect(name.c_str(), false, 1);
}

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
