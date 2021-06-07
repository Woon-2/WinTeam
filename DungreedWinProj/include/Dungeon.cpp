#include "Dungeon.h"

Dungeon::Dungeon()
{
	DataBase* db = BuildDB();

	db->LoadDataByFirstID();

	dungeon_image = new Image(map_path);
	dungeon_terrain_image = new Image(map_terrain_path);

	delete db;
}

Dungeon::Dungeon(const int dungeon_id) : dungeon_id{ dungeon_id }
{
	DataBase* db = BuildDB();

	db->LoadDataByID(dungeon_id);

	dungeon_image = new Image(map_path);
	dungeon_terrain_image = new Image(map_terrain_path);

	delete db;
}


// -------------------------------------------------------------------
// DB와 던전의 멤버 변수를 수정하게 된다면 반드시 이 함수도 수정해야 함!!
// -------------------------------------------------------------------
DataBase* Dungeon::BuildDB()
{
	DataBase* db = new DataBase(L"DungeonData.txt");

	db->RegisterField("map_path", &map_path);
	db->RegisterField("map_terrain_path", &map_terrain_path);
	db->RegisterField("left_start_pos", &left_start_pos);
	db->RegisterField("right_start_pos", &right_start_pos);
	db->RegisterField("prev_dungeon_portal_pos", &prev_dungeon_portal_pos);
	db->RegisterField("next_dungeon_portal_pos", &next_dungeon_portal_pos);
	db->RegisterField("dungeon_width", &dungeon_width);
	db->RegisterField("dungeon_height", &dungeon_height);
	db->RegisterField("next_dungeon_id", &next_dungeon_id);
	db->RegisterField("prev_dungeon_id", &prev_dungeon_id);
	db->RegisterField("camera_x_half_range", &camera_x_half_range);
	db->RegisterField("camera_y_half_range", &camera_y_half_range);

	return db;
}

Dungeon::~Dungeon()
{
	delete dungeon_image;
	delete dungeon_terrain_image;
}

void Dungeon::Render(HDC scene_dc, const RECT& bit_rect) const
{
	dungeon_image->Draw(scene_dc, 0, 0, bit_rect.right, bit_rect.bottom, 0, 0, dungeon_width, dungeon_height);
}