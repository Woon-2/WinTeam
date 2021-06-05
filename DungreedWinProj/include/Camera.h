#pragma once
#ifndef _camera
#define _camera
#include <windows.h>
#include "Player.h"
#include "Dungeon.h"

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
	inline RECT Rect() { return RECT{ pos.x - x_half_range, pos.y - y_half_range, pos.x + x_half_range, pos.y + y_half_range }; }

	friend class Scene;
};
#endif
