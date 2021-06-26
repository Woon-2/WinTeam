#pragma once
#ifndef _player
#define _player
#include "Character.h"
#include "Uncopyable.h"
#include "InstantDCSet.h"
#include "Crosshair.h"
#include "Sound.h"
#include "Weapon.h"
#include "HitScan.h"
#include <cstring>
#include "Effect.h"

extern HDC buf_dc;
extern RECT client;

const double PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE = 10.0;
const double PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE = 5.0;

class Crosshair;
class Weapon;

class Player : private Uncopyable, public Character
{
private:
	double dash_power = 0;	// dash_power > 0 이면 dash중인 상태, dash_power < 0 이면 다음 dash 가능 시간까지 대기중, dash_power == 0이면 dash 가능 상태
	double dash_radian = 0;

	double atk_radian;

	int walk_cnt = 0;

	bool is_doing_missile_attack;

	void KeyProc(const Dungeon* dungeon, MissileManager* missile_manager, SoundManager* sound_manager);
	void DashProc(float radian, const Dungeon* dungeon, const int px, SoundManager* sound_manager);
	void AttackProc(Weapon* weapon, const Crosshair* crosshair, MissileManager* missile_manager, AnimationManager* animation_manager, SoundManager* sound_manager);

	void MatchStateAndAnimation(AnimationManager* animation_manager, SoundManager* sound_manager, EffectManager* effect_manager);

public:
	Player(const Dungeon* dungeon, AnimationManager* animation_manager) :
		Character(1234567, dungeon->camera_x_half_range / PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE,
			dungeon->camera_x_half_range / PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE,
			dungeon->left_start_pos, State::DOWN, TRUE,
			dungeon->camera_x_half_range / 60.0f, dungeon->camera_y_half_range / 32.0f, "player_stand",
			L"animation/player_stand1.png", 200, 100, 50, animation_manager)
	{
		animation.LoadAnimation(animation_manager, "player_stand");
		animation.Play();
		strcpy_s(atk_sound_name, "sound\\Slash2.ogg");
		atk_sound_volume = 0.4f;
	}

	void PlaceWithDungeonLeft(const Dungeon* dungeon);
	void PlaceWithDungeonRight(const Dungeon* dungeon);

	void Init(const Dungeon* dungeon, AnimationManager* animation_manager);

	void Update(const Dungeon* dungeon, Weapon* weapon, const Crosshair* crosshair, MissileManager* missile_manager, AnimationManager* animation_manager, SoundManager* sound_manager, EffectManager* effect_manager);

	friend class Camera;
	friend class Weapon;
	friend class MonsterAI;
};
#endif

