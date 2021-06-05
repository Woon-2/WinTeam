#pragma once
#ifndef _player
#define _player
#include "Character.h"

const double PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE = 10.0;
const double PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE = 5.0;

class Player : public Character
{
private:
public:
	Player(const Dungeon* dungeon);
	void PlaceWithDungeonLeft(const Dungeon* dungeon);
	void PlaceWithDungeonRight(const Dungeon* dungeon);

	void Init(const Dungeon* dungeon);
	void Render();

	void KeyMove(HDC, const TCHAR*);

	friend class Camera;
};
#endif

