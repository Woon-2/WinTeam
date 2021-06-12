#pragma once
#ifndef _camera
#define _camera
#include <windows.h>
#include "Player.h"
#include "Dungeon.h"

class Player;

class Camera
{
private:
	POINT pos;
	int x_half_range;
	int y_half_range;

public:
	Camera(const Dungeon* dungeon, const Player* player);

	void Init(const Dungeon* dungeon, const Player* player);
	void Update(const Dungeon* dungeon, const Player* player);
	inline RECT Rect() const { return RECT{ static_cast<int>(pos.x - x_half_range), static_cast<int>(pos.y - y_half_range), static_cast<int>(pos.x + x_half_range), static_cast<int>(pos.y + y_half_range) }; }

	friend class Crosshair;
	friend class Weapon;
};
#endif
