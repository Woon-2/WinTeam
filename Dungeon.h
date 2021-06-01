#pragma once
#include <Windows.h>
#include "FileUtility.h"

struct Dungeon
{
	Image dungeon_image;
	POINT start_pos;
	POINT prev_dungeon_portal_pos;
	POINT next_dungeon_portal_pos;

	int dungeon_width;
	int dungeon_height;

	Dungeon* prev, * next;
};