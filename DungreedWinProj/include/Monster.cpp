#include "Monster.h"

void Monster::Update(const Dungeon* dungeon)
{
	//AutoAction 루틴
	ForceGravity(dungeon);
}

MonsterManager::MonsterManager(const Dungeon* dungeon)
{
	for (int i = 0; i < MAX_MONSTER_KIND_IN_DUNGEON; ++i)
		if (dungeon->monster_ids[i])
			Insert(dungeon, dungeon->monster_ids[i], dungeon->monster_nums[i]);
		else
			break;
}

void MonsterManager::Init(const Dungeon* dungeon)
{
	Clear();
	for (int i = 0; i < MAX_MONSTER_KIND_IN_DUNGEON; ++i)
		if (dungeon->monster_ids[i])
			Insert(dungeon, dungeon->monster_ids[i], dungeon->monster_nums[i]);
		else
			break;
}

MonsterManager::~MonsterManager()
{
	for (auto* monster : monsters)
		delete monster;
}

void MonsterManager::Insert(const Dungeon* dungeon, const int monster_id, int num)
{
	auto monster_db = BuildDB();
	POINT pos;
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	monster_db->Load(monster_id);

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);
	std::uniform_int_distribution<> uid_x{ 0, dungeon->dungeon_width };
	std::uniform_int_distribution<> uid_y{ 0, dungeon->dungeon_height };

	do {
		pos.x = uid_x(dre);
		pos.y = uid_y(dre);
	} while (!MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 255), pos));

	LoadNeededAnimations();
	
	TCHAR start_image_path[FILE_NAME_LEN] = L"animation/";
	lstrcat(start_image_path, stand_animation_name_tstr);
	lstrcat(start_image_path, L"1.png");

	Tstr2Str(stand_animation_name_tstr, stand_animation_name);
	Tstr2Str(attack_animation_name_tstr, attack_animation_name);
	Tstr2Str(move_animation_name_tstr, move_animation_name);

	while (num--) {
		Monster* monster = new Monster(monster_id, width, height, pos, x_move_px, jump_start_power,
			hp, atk, def, is_floating, melee_attack, missile_attack,
			stand_animation_name, attack_animation_name, move_animation_name, start_image_path); // = new Monster(...)
		monsters.push_back(monster);
	}

	BufferEmpty();
}

void MonsterManager::LoadNeededAnimations()
{
	auto animation_db = std::shared_ptr<DB::DataBase>(new DB::DataBase(L"AnimationData.txt"));

	if (animation_ids[0]) {
		animation_db->RegisterField("animation_name", &stand_animation_name_tstr);
		animation_db->Load(animation_ids[0]);
	}
	if (animation_ids[1]) {
		animation_db->RegisterField("animation_name", &attack_animation_name_tstr);
		animation_db->Load(animation_ids[1]);
	}
	if (animation_ids[2]) {
		animation_db->RegisterField("animation_name", &move_animation_name_tstr);
		animation_db->Load(animation_ids[2]);
	}
}

void MonsterManager::BufferEmpty()
{
	stand_animation_name = "";
	attack_animation_name = "";
	move_animation_name = "";
	lstrcpy(stand_animation_name_tstr, L"");
	lstrcpy(attack_animation_name_tstr, L"");
	lstrcpy(move_animation_name_tstr, L"");
}

void MonsterManager::Clear()
{
	for (Monster* monster : monsters)
		delete monster;
	monsters.clear();
}

std::shared_ptr<DB::DataBase> MonsterManager::BuildDB()
{
	auto db = std::shared_ptr<DB::DataBase>(new DB::DataBase(L"MonsterData.txt"));

	db->RegisterField("monster_name", &monster_name);
	db->RegisterField("width", &width);
	db->RegisterField("height", &height);
	db->RegisterField("x_move_px", &x_move_px);
	db->RegisterField("jump_start_power", &jump_start_power);
	db->RegisterField("hp", &hp);
	db->RegisterField("atk", &atk);
	db->RegisterField("def", &def);
	db->RegisterField("is_floating", &is_floating);
	db->RegisterField("melee_attack", &melee_attack);
	db->RegisterField("missile_attack", &missile_attack);
	db->RegisterField("monster_name", &monster_name);

	for (int i = 0; i < MONSTER_MAX_ANIMATION_NUM; ++i) {
		std::string field_str = "animation_id";
		std::stringstream idx;
		idx << i;
		field_str += idx.str();
		db->RegisterField(field_str, &animation_ids[i]);
	}
		
	return db;
}

bool MonsterManager::MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt)	// 지형 표시 이미지를 사용해 충돌 확인, 오류 있음
{
	if (pt.x < client.left || pt.y > client.right)
		return false;
	if (pt.y < client.top || pt.y > client.bottom)
		return false;

	if (GetPixel(terrain_dc, pt.x, pt.y) == val)
		return true;
	else
		return false;
}