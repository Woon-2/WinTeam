#pragma once
#ifndef _monster
#define _monster
#include "Character.h"
#include "Uncopyable.h"
#include <vector>
#include "DataBase.h"
#include "FileUtility.h"
#include <map>
#include "InstantDCSet.h"
#include "Dungeon.h"
#include <random>

extern std::default_random_engine dre;
constexpr int MONSTER_MAX_ANIMATION_NUM = 4;

class Monster : public Character
{
private:
	BOOL is_floating;
	BOOL melee_attack;
	BOOL missile_attack;

	const std::string stand_animation_name;
	const std::string attack_animation_name;
	const std::string move_animation_name;

public:
	Monster(const int monster_id, const int width, const int height, const POINT pos,
		const int x_move_px, const int jump_start_power,
		const int hp, const int atk, const int def, const BOOL is_floating, const BOOL melee_attack, const BOOL missile_attack,
		const std::string& stand_animation_name, const std::string& attack_animation_name, const std::string& move_animation_name,
		const TCHAR* start_image_path)
		: Character(monster_id, width, height, pos, State::DOWN, TRUE, x_move_px, jump_start_power,
			stand_animation_name, start_image_path, hp, atk, def),
		is_floating {is_floating}, melee_attack {melee_attack}, missile_attack {missile_attack},
		stand_animation_name{ stand_animation_name }, attack_animation_name{ attack_animation_name }, move_animation_name{ move_animation_name } {}

	void Update(const Dungeon* dungeon);

	friend class MonsterManager;
};

class MonsterManager : private Uncopyable {
private:
	std::vector<Monster*> monsters;

	std::string monster_name;
	int width;
	int height;
	int x_move_px;
	int jump_start_power;
	int hp;
	int atk;
	int def;
	BOOL is_floating;
	BOOL melee_attack;
	BOOL missile_attack;

	int animation_ids[MONSTER_MAX_ANIMATION_NUM];
	std::string stand_animation_name = "";
	std::string attack_animation_name = "";
	std::string move_animation_name = "";
	TCHAR stand_animation_name_tstr[FILE_NAME_LEN];
	TCHAR attack_animation_name_tstr[FILE_NAME_LEN];
	TCHAR move_animation_name_tstr[FILE_NAME_LEN];

	std::shared_ptr<DB::DataBase> BuildDB();

	void Insert(const Dungeon* dungeon, const int monster_id, int num);
	void LoadNeededAnimations();
	void BufferEmpty();
	void Clear();

	bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
public:
	MonsterManager(const Dungeon* dungeon);
	~MonsterManager();

	void Init(const Dungeon* dungeon);
};
#endif
