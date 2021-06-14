#include "Monster.h"

void Monster::Update(const Dungeon* dungeon)
{
	if (is_appeared) {
		AutoAction();
		// Die 루틴
		ForceGravity(dungeon);
		ForceGravity(dungeon);
		// UpdateAnimation(animation_manager);
	}
}

void Monster::AutoAction()
{
	if (!remain_update_cnt_to_change_action)
		ChooseNewPolicy();
	else {
		FollowPolicy();
		--remain_update_cnt_to_change_action;
	}
}

void Monster::FollowPolicy()
{
	switch (cur_policy) {
	case Policy::STAND:
		Stand();
		break;
	case Policy::MOVE_TO_PLAYER:
		MoveToPlayer();
		break;
	case Policy::MOVE_FROM_PLAYER:
		MoveFromPlayer();
		break;
	case Policy::ATTACK:
		Attack();
		break;
	}
}

void Monster::Stand()
{

}

void Monster::MoveToPlayer()
{

}

void Monster::MoveFromPlayer()
{

}

void Monster::Attack()
{

}

void Monster::ChooseNewPolicy()
{
	std::uniform_int_distribution<> uid_chance{ 1, 100 };
	int chance = uid_chance(dre);

	if (chance <= policy_stand.x) {
		cur_policy = Policy::STAND;
		remain_update_cnt_to_change_action = policy_stand.y;
		return;
	}
	chance -= policy_stand.x;
	if (chance <= policy_move_to_player.x) {
		cur_policy = Policy::MOVE_TO_PLAYER;
		remain_update_cnt_to_change_action = policy_move_to_player.y;
		return;
	}
	chance -= policy_move_to_player.x;
	if (chance <= policy_move_from_player.x) {
		cur_policy = Policy::MOVE_FROM_PLAYER;
		remain_update_cnt_to_change_action = policy_move_from_player.y;
		return;
	}
	chance -= policy_move_from_player.x;
	if (chance <= policy_attack.x) {
		cur_policy = Policy::ATTACK;
		remain_update_cnt_to_change_action = policy_attack.y;
		return;
	}
}

void Monster::ForceGravity(const Dungeon* dungeon)
{
	if (!is_floating)
		this->Character::ForceGravity(dungeon);
}

void Monster::Render(HDC scene_dc, const RECT& bit_rect) const
{
	if (is_appeared)
		this->Character::Render(scene_dc, bit_rect);
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

void MonsterManager::Update(const Dungeon* dungeon)
{
	for (Monster* monster : monsters) {
		monster->Update(dungeon);
		// Die 루틴 : 현재는 죽으면 그냥 출현 취소
		if (monster->is_appeared && monster->IsDied()) {
			monster->is_attacking = false;
			monster->is_appeared = false;
			--remain_monster_cnt;
		}
	}
}

void MonsterManager::Render(HDC scene_dc, const RECT& bit_rect) const
{
	for (Monster* monster : monsters)
		monster->Render(scene_dc, bit_rect);
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

	// 이 수치들은 카메라에 대한 천분율
	width *= dungeon->camera_x_half_range / 1000.0f;
	height *= dungeon->camera_y_half_range / 1000.0f;
	x_move_px *= dungeon->camera_x_half_range / 1000.0f;
	jump_start_power *= dungeon->camera_y_half_range / 1000.0f;
	//

	// 시작 위치는 지형 상 핑크색 위치중 랜덤
	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);
	std::uniform_int_distribution<> uid_x{ 0, dungeon->dungeon_width };
	std::uniform_int_distribution<> uid_y{ 0, dungeon->dungeon_height / 3 * 2 };

	LoadNeededAnimations();
	
	TCHAR start_image_path[FILE_NAME_LEN] = L"animation/";
	lstrcat(start_image_path, stand_animation_name_tstr);
	lstrcat(start_image_path, L"1.png");

	Tstr2Str(stand_animation_name_tstr, stand_animation_name);
	Tstr2Str(attack_animation_name_tstr, attack_animation_name);
	Tstr2Str(move_animation_name_tstr, move_animation_name);

	while (num--) {
		do {
			pos.x = uid_x(dre);
			pos.y = uid_y(dre);
		} while (!MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 255), pos));

		Monster* monster = new Monster(monster_id, width, height, pos, x_move_px, jump_start_power,
			hp, atk, def, is_floating, melee_attack, missile_attack,
			stand_animation_name, attack_animation_name, move_animation_name, start_image_path,
			policy_stand, policy_move_to_player, policy_move_from_player, policy_attack); // = new Monster(...)
		monsters.push_back(monster);
	}

	remain_monster_cnt += num;
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
	db->RegisterField("policy_stand", &policy_stand);
	db->RegisterField("policy_move_to_player", &policy_move_to_player);
	db->RegisterField("policy_move_from_player", &policy_move_from_player);
	db->RegisterField("policy_attack", &policy_attack);

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

void MonsterManager::Appear(int num)
{
	// 일정 시간 이후 문제가 생긴다면 어쩌면 여기 문제
	std::shuffle(monsters.begin(), monsters.end(), dre);
	for (Monster* monster : monsters)
		if (!monster->is_appeared) {
			monster->is_appeared = true;
			if (--num == 0)
				break;
		}
}