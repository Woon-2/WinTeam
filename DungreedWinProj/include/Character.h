#pragma once
#ifndef _character
#define _character
#include <windows.h>
#include "FileUtility.h"

class Character
{
private:
	int width;
	int height;
	POINT pos;

	int jump_power;
	int state;

	int motion_num;
	Image* image;
public:
	enum state {
		DOWN, UP, LANDING, DOWNJUMP
	};

	inline void SetPos(POINT pos) { this->pos = pos; }
	inline const POINT GetPos() const { return pos; }
	inline void SetState(int state) { this->state = state; }
	inline const int GetState() const { return state; }
	inline void SetJumpPower(int jump_power) { this->jump_power = jump_power; }
	inline const int GetWidth() const { return width; }
	inline const int GetHeight() const { return height; }

	void Init();

	BOOL MapPixelCollision(const TCHAR*, COLORREF, int, int, HDC);
	void Render(HDC&);
	void Update();

	void Move(HDC, const TCHAR*);
};
#endif
