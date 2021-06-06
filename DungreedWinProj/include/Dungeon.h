#pragma once
#ifndef _dungeon
#define _dungeon
#include <windows.h>
#include "FileUtility.h"
#include "Uncopyable.h"

extern HDC buf_dc;
extern RECT client;
extern HWND h_wnd;

class Dungeon : private Uncopyable
{
private:
	int dungeon_id;

	DataBase* BuildDB();

public:
	Image* dungeon_image;
	Image* dungeon_terrain_image;

	TCHAR map_path[DEF_STR_LEN];
	TCHAR map_terrain_path[DEF_STR_LEN];

	POINT left_start_pos;
	POINT right_start_pos;
	POINT prev_dungeon_portal_pos;
	POINT next_dungeon_portal_pos;

	int dungeon_width;
	int dungeon_height;

	int next_dungeon_id;
	int prev_dungeon_id;

	int camera_x_half_range;
	int camera_y_half_range;

	Dungeon();
	Dungeon(const int dungeon_id);
	~Dungeon();

	void Render(HDC scene_dc, const RECT& bit_rect);
};
#endif