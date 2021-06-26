#include "Sound.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	Release();
	SystemClose();
	SystemRelease();
}

void SoundManager::Init()
{
	FMOD_System_Create(&system);
	FMOD_System_Init(system, TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);
	bgm = new Sound;
}

void SoundManager::PlayBgm(const char* bgm_name)
{
	FMOD_Channel_Stop(bgm->channel);
	bgm->name = bgm_name;
	FMOD_System_CreateSound(system, bgm_name, FMOD_LOOP_NORMAL, 0, &bgm->sound);
	FMOD_System_PlaySound(system, bgm->sound, NULL, 0, &bgm->channel);
	FMOD_Channel_SetVolume(bgm->channel, 0.5);
}

void SoundManager::InsertEffectSound(const char* effect_name)
{
	Sound* new_effect = new Sound;
	new_effect->name = effect_name;
	FMOD_System_CreateSound(system, effect_name, FMOD_DEFAULT, 0, &new_effect->sound);
	effect_sounds.push_back(new_effect);
}

void SoundManager::Play(const char* effect_name)
{
	for (int i = 0; i < effect_sounds.size(); i++)
	{
		if (!strcmp(effect_sounds[i]->name.c_str(), effect_name)) {
			//FMOD_Channel_Stop(effect_sounds[i]->channel);
			FMOD_System_PlaySound(system, effect_sounds[i]->sound, NULL, 0, &effect_sounds[i]->channel);
			FMOD_Channel_SetVolume(effect_sounds[i]->channel, 1.0);
			break;
		}
	}
}

void SoundManager::Play(const char* effect_name, const float volume)
{
	for (int i = 0; i < effect_sounds.size(); i++)
	{
		if (!strcmp(effect_sounds[i]->name.c_str(), effect_name)) {
			//FMOD_Channel_Stop(effect_sounds[i]->channel);
			FMOD_System_PlaySound(system, effect_sounds[i]->sound, NULL, 0, &effect_sounds[i]->channel);
			FMOD_Channel_SetVolume(effect_sounds[i]->channel, volume);
			break;
		}
	}
}


void SoundManager::Release()
{
	FMOD_Sound_Release(bgm->sound);

	for (int i = effect_sounds.size() - 1; i >= 0; --i)
	{
		delete effect_sounds[i];
		FMOD_Sound_Release(effect_sounds[i]->sound);
		effect_sounds.erase(effect_sounds.begin() + i);
	}

}
void SoundManager::SystemClose()
{
	FMOD_System_Close(system);
}
void SoundManager::SystemRelease()
{
	FMOD_System_Release(system);
}