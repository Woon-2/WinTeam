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
	void KeyProc(const Dungeon* dungeon);

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

