#pragma once
#ifndef _missile
#define _missile
#include "FileUtility.h"
#include "Windows.h"
#include "Character.h"
#include <math.h>

extern bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
extern bool CanGoToPos(const HDC terrain_dc, const POINT pos);

class Missile {
private:
	Character* host;
	Image image;
	POINT pos;
	int width;
	int height;
	float radian;
	int speed;
	BOOL looking_direction;

public:
	Missile(Character* host, const Image& image, const POINT pos, const int width, const int height, const float radian,
		const int speed, const BOOL looking_direction)
		: host{ host }, image{ image }, pos{ pos }, width{ width }, height{ height }, radian{ radian },
		speed{ speed }, looking_direction{ looking_direction } {}
	void Update(float radian, const Dungeon* dungeon);
	void Render(HDC scene_dc) const;

	bool IsOut_Left(const Dungeon* dungeon) const;
	bool IsOut_Right(const Dungeon* dungeon) const;
};

#endif