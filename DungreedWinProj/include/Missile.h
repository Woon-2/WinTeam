#pragma once
#ifndef _missile
#define _missile
#include "FileUtility.h"
#include "Windows.h"
#include "Character.h"
#include <math.h>
#include <vector>
#include "Animation.h"
#include <string>
#include <cstring>
#define pi 3.141592

extern bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
extern bool CanGoToPos(const HDC terrain_dc, const POINT pos);

class Missile {
private:
	const Image* image;
	const Image* start_image;

	Animation animation;

	bool is_animation_load_requested = false;
	std::string animation_name;

	POINT old_pos;
	POINT pos;
	int width;
	int height;
	float radian;
	int speed;
	int range;
	BOOL looking_direction;
	int pierce;					// °üÅë·Â
	int atk;

	std::vector<const Character*> attack_victims;
	RECT atk_rect;
	char atk_sound_name[FILE_NAME_LEN];
	float atk_sound_volume;

	bool IsOutOfRange() const;

	void UpdateAnimation(AnimationManager* animation_manager);

	void AddAttackVictim(const Character* victim);
	bool HasAlreadyAttacked(const Character* victim) const;
public:
	const Character* host;

	Missile(Character* host, const POINT pos, const int width, const int height, const float radian,
		const int speed, const int range, const BOOL looking_direction, const int pierce, const int atk, const TCHAR* start_image_path,
		const std::string& start_animation_name, AnimationManager* animation_manager)
		: host{ host }, pos{ pos }, old_pos{ pos }, width{ width }, height{ height }, radian{ radian },
		speed{ speed }, range{ range }, looking_direction{ looking_direction }, pierce{ pierce }, atk{ atk }
	{
		animation.LoadAnimation(animation_manager, start_animation_name);
		animation_name = start_animation_name;
		image = start_image = new Image(start_image_path);
		animation.Play();
	}

	Missile(Character* host, const POINT pos, const int width, const int height, const float radian,
		const int speed, const int range, const BOOL looking_direction, const int pierce, const int atk, const TCHAR* start_image_path,
		const std::string& start_animation_name, AnimationManager* animation_manager, const char* hit_sound, const float hit_sound_volume)
		: host{ host }, pos{ pos }, old_pos{ pos }, width{ width }, height{ height }, radian{ radian },
		speed{ speed }, range{ range }, looking_direction{ looking_direction }, pierce{ pierce }, atk{ atk }, atk_sound_volume {hit_sound_volume}
	{
		animation.LoadAnimation(animation_manager, start_animation_name);
		animation_name = start_animation_name;
		image = start_image = new Image(start_image_path);
		animation.Play();
		strcpy_s(atk_sound_name, hit_sound);
	}

	~Missile()
	{
		delete start_image;
	}

	void Update(AnimationManager* animation_manager);
	void Render(HDC scene_dc, const RECT& bit_rect) const;

	bool IsOut_Left(const Dungeon* dungeon) const;
	bool IsOut_Right(const Dungeon* dungeon) const;

	friend class MissileManager;
	friend class HitScanner;
};

class MissileManager {
private:
public:
	std::vector<Missile*>  missiles;

	void Init();
	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void Update(const Dungeon* dungeon, AnimationManager* animation_manager);
	void Insert(Missile* given_missile);
	void Delete(Missile* given_missile);
};

#endif