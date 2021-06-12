#pragma once
#ifndef _player
#define _player
#include "Character.h"
#include "Uncopyable.h"
#include "InstantDCSet.h"
#include "Crosshair.h"

extern HDC buf_dc;
extern RECT client;

const double PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE = 10.0;
const double PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE = 5.0;

class Crosshair;

class Player : private Uncopyable, public Character
{
private:
	double dash_power = 0;	// dash_power > 0 이면 dash중인 상태, dash_power < 0 이면 다음 dash 가능 시간까지 대기중, dash_power == 0이면 dash 가능 상태
	double dash_radian = 0;

	void KeyProc(const Dungeon* dungeon);
	void DashProc(float radian, const Dungeon* dungeon, const int px);

	void MatchStateAndAnimation(AnimationManager* animation_manager);

public:
	Player(const Dungeon* dungeon, AnimationManager* animation_manager) :
		Character(dungeon->camera_x_half_range / PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE,
			dungeon->camera_x_half_range / PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE,
			dungeon->left_start_pos, State::DOWN, TRUE,
			dungeon->camera_x_half_range / 60.0f, dungeon->camera_y_half_range / 32.0f, "player_stand",
			L"animation/player_stand1.png")
	{
		animation_manager->Play("player_stand");
	}

	void PlaceWithDungeonLeft(const Dungeon* dungeon);
	void PlaceWithDungeonRight(const Dungeon* dungeon);

	void Init(const Dungeon* dungeon, AnimationManager* animation_manager);

	void Update(const Dungeon* dungeon, const Crosshair* crosshair, AnimationManager* animation_manager);

	friend class Camera;
	friend class Weapon;
};
#endif

