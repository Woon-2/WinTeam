#pragma once
#ifndef _weapon
#define _weapon
#include <windows.h>
#include <math.h>
#include "Crosshair.h"
#include "FileUtility.h"

class Weapon
{
private:
	Image* image;
	POINT pos;
	int width;
	int height;
	float angle;
public:
	Weapon(const Dungeon* dungeon, const POINT* player, const int* player_width, const int* player_height, const POINT* crosshair);
	void Update(const POINT* player, const int* player_width, const int* player_height, const POINT* crosshair);
	void Render(HDC scene_dc, const RECT& bit_rect, BOOL looking_direction);
};
#endif