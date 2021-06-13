#pragma once
#ifndef _dungeon
#define _dungeon
#include <windows.h>
#include "DataBase.h"
#include "Uncopyable.h"
#include "FileUtility.h"
#include <string>
#include <sstream>

extern HDC buf_dc;
extern RECT client;
extern HWND h_wnd;

constexpr int MAX_MONSTER_KIND_IN_DUNGEON = 6;

class Dungeon : private Uncopyable
{
private:
	int dungeon_id;

	std::shared_ptr<DB::DataBase> BuildDB();

public:
	Image* dungeon_image;
	Image* dungeon_terrain_image;

	TCHAR map_path[FILE_NAME_LEN];
	TCHAR map_terrain_path[FILE_NAME_LEN];

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

	int monster_ids[MAX_MONSTER_KIND_IN_DUNGEON];
	int monster_nums[MAX_MONSTER_KIND_IN_DUNGEON];

	Dungeon();
	Dungeon(const int dungeon_id);
	~Dungeon();

	bool CanGoPrev() const;
	bool CanGoNext() const;

	void Render(HDC scene_dc, const RECT& bit_rect) const;
};
#endif