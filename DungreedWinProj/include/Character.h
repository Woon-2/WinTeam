#pragma once
#ifndef _character
#define _character
#include <windows.h>
#include "FileUtility.h"
#include "Dungeon.h"
#include "InstantDCSet.h"

extern HDC buf_dc;
extern HWND h_wnd;
extern RECT client;

class Character
{
protected:
	enum class State {
		DOWN, UP, LANDING, DOWNJUMP
	};
	enum class Direction {
		LEFT, UP, RIGHT, DOWN
	};

	int width;
	int height;
	POINT pos;

	int jump_power;

	State state;

	int motion_num;
	Image* image;

	bool MapPixelCollision(HDC terrain_dc, COLORREF val, POINT pt);
	void MovePos(const Dungeon* dungeon, Direction direction, const int px);
public:
	void Update();

	void ForceGravity(const Dungeon* dungeon);

	void Render(HDC scene_dc, const RECT& bit_rect, BOOL looking_direction );
};
#endif
