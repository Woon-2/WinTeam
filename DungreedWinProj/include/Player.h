#pragma once
#ifndef _player
#define _player
#include "Character.h"
#include "Uncopyable.h"
#include "InstantDCSet.h"

extern HDC buf_dc;

const double PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE = 10.0;
const double PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE = 5.0;

class Player : private Uncopyable, public Character
{
private:
public:
	Player(const Dungeon* dungeon);
	void PlaceWithDungeonLeft(const Dungeon* dungeon);
	void PlaceWithDungeonRight(const Dungeon* dungeon);

	void Init(const Dungeon* dungeon);

	void KeyProc(const Dungeon* dungeon);

	friend class Camera;
	friend class Scene;
};
#endif

