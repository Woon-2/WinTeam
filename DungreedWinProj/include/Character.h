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

	double jump_power;

	State state;

	int motion_num;
	Image* image;

	BOOL looking_direction;	// TRUE면 오른쪽 보는 상태, FALSE면 왼쪽 보고 있는 상태

	bool MapPixelCollision(HDC terrain_dc, const COLORREF& val, const POINT& pt);
	void MovePos(const Dungeon* dungeon, Direction direction, const int px);
public:
	virtual void Update(const Dungeon* dungeon) {}

	void ForceGravity(const Dungeon* dungeon);

	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void Look(const POINT& target);
};
#endif
