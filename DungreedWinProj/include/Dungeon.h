#pragma once
#ifndef _dungeon
#define _dungeon
#include <windows.h>
#include "FileUtility.h"
#include <string.h>

class Dungeon
{
private:
	int dungeon_id;
public:
	Image dungeon_image;

	POINT left_start_pos;
	POINT right_start_pos;
	POINT prev_dungeon_portal_pos;
	POINT next_dungeon_portal_pos;

	int dungeon_width;
	int dungeon_height;

	int next_dungeon_id;
	int prev_dungeon_id;
	
	Dungeon() = default;
	Dungeon(const int dungeon_id, int& camera_x_half_range, int& camera_y_half_range);
};
#endif