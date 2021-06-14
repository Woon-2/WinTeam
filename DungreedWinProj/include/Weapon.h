#pragma once
#ifndef _weapon
#define _weapon
#include <windows.h>
#include <math.h>
#include "Crosshair.h"
#include "FileUtility.h"
#include "Player.h"
#include "Crosshair.h"

class Weapon
{
private:
	Image image;
	POINT pos;
	int width;
	int height;
	float angle;
	BOOL looking_direction;

public:  
	Weapon(const Camera* camera, const Player* player, const Crosshair* crosshair);
	void Init(const Camera* camera, const Player* player, const Crosshair* crosshair);
	void Update(const Player* player, const Crosshair* crosshair);
	void Render(HDC scene_dc, const RECT& bit_rect);

	friend class HitScanner;
	inline RECT AtkRect() const { return RECT{ pos.x, pos.y, pos.x + width, pos.y + height }; }
};
#endif