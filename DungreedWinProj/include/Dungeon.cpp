#include "Dungeon.h"

Dungeon::Dungeon(const int dungeon_id) : dungeon_id{ dungeon_id }
{
	CheckFileNameValidity(L"DungeonData.txt");
	std::ifstream in;
	// 데이터베이스에 있는 정보대로 던전 로드
}