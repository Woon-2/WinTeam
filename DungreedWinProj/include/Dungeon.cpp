#include "Dungeon.h"
#include <iostream>

Dungeon::Dungeon()
{
	auto db = BuildDB();

	db->Load();

	dungeon_image = new Image(map_path);
	dungeon_terrain_image = new Image(map_terrain_path);
}

Dungeon::Dungeon(const int dungeon_id) : dungeon_id{ dungeon_id }
{
	auto db = BuildDB();

	db->Load(dungeon_id);

	dungeon_image = new Image(map_path);
	dungeon_terrain_image = new Image(map_terrain_path);
}

Dungeon::~Dungeon()
{
	delete dungeon_image;
	delete dungeon_terrain_image;
}


// -------------------------------------------------------------------
// DB와 던전의 멤버 변수를 수정하게 된다면 반드시 이 함수도 수정해야 함!!
// -------------------------------------------------------------------
std::shared_ptr<DB::DataBase> Dungeon::BuildDB()
{
	auto db = std::shared_ptr<DB::DataBase>(new DB::DataBase(L"DungeonData.txt"));

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

	for (int i = 0; i < MAX_MONSTER_KIND_IN_DUNGEON; ++i) {
		std::string id_field_str = "monster_id";
		std::string num_field_str = "monster_num";
		std::stringstream idx;
		idx << i;
		id_field_str += idx.str();
		num_field_str += idx.str();
		db->RegisterField(id_field_str, &monster_ids[i]);
		db->RegisterField(num_field_str, &monster_nums[i]);
	}

	return db;
}

bool Dungeon::CanGoNext() const
{
	if (!next_dungeon_id)
		return false;
	else
		return true;
}

bool Dungeon::CanGoPrev() const
{
	if (!prev_dungeon_id)
		return false;
	else
		return true;
}

void Dungeon::Render(HDC scene_dc, const RECT& bit_rect) const
{
	dungeon_image->Draw(scene_dc, 0, 0, bit_rect.right, bit_rect.bottom, 0, 0, dungeon_width, dungeon_height);
}