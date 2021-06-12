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
	double dash_power;	// dash_power > 0 이면 dash중인 상태, dash_power < 0 이면 다음 dash 가능 시간까지 대기중, dash_power == 0이면 dash 가능 상태
	double dash_radian;

	void KeyProc(const Dungeon* dungeon);
	void Dash(float radian, const Dungeon* dungeon, const int px);
public:
	Player(const Dungeon* dungeon);
	void PlaceWithDungeonLeft(const Dungeon* dungeon);
	void PlaceWithDungeonRight(const Dungeon* dungeon);

	void Init(const Dungeon* dungeon);

	void Update(const Dungeon* dungeon, const Crosshair* crosshair);

	bool IsOut_Left(const Dungeon* dungeon) const;
	bool IsOut_Right(const Dungeon* dungeon) const;

	friend class Camera;
	friend class Weapon;
};
#endif

