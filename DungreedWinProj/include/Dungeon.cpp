#include "Dungeon.h"

Dungeon::Dungeon(const int dungeon_id, int& camera_x_half_range, int& camera_y_half_range) : dungeon_id{ dungeon_id }
{
	CheckFileNameValidity(L"DungeonData.txt");
	// 데이터베이스에 있는 정보대로 던전 로드
}