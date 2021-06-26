#pragma once
#ifndef _sound
#define _sound
#include <iostream>
#include <fmod.h>
#include <vector>
#include <string>

#define SOUNDBUFFER 10
#define EXTRABUFFER 5

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRABUFFER

class Sound
{
private:
	std::string name;

	FMOD_SOUND* sound;
	FMOD_CHANNEL* channel;
	bool loop;

	friend class SoundManager;
};

typedef std::vector<Sound*>Sounds;
typedef std::vector<Sound*>::iterator ViSounds;

class SoundManager
{
private:
	FMOD_SYSTEM* system;

	Sound* bgm;
	Sounds effect_sounds;
public:
	SoundManager();
	~SoundManager();

	void Init();
	void PlayBgm(const char* bgm_name);	// bgm 재생, bgm 재생 중 PlayBgm(새로운 파일 경로 + 파일 이름) 하면 새로운 bgm으로 바뀜

	void InsertEffectSound(const char* effect_name);
	void Play(const char* effect_name);
	void Play(const char* effect_name, const float volume);
	//void Stop(std::string name);
	void Release();
	void SystemClose();
	void SystemRelease();
};
#endif