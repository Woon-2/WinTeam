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
#include <algorithm>
#include "Player.h"
#include "MonsterAI.h"
#include "Animation.h"

extern std::default_random_engine dre;
constexpr int MONSTER_MAX_ANIMATION_NUM = 4;

class MonsterAI;

class Monster : public Character
{
private:
	enum class Policy {STAND, MOVE_TO_PLAYER, MOVE_FROM_PLAYER, ATTACK};

	BOOL is_floating;
	BOOL melee_attack;
	BOOL missile_attack;

	const std::string stand_animation_name;
	const std::string attack_animation_name;
	const std::string move_animation_name;

	bool is_appeared = false;


	POINT policy_stand;					// x는 행동을 선택할 확률, y는 행동을 몇 업데이트 카운트 동안 지속할 건지
	POINT policy_move_to_player;
	POINT policy_move_from_player;
	POINT policy_attack;
	Policy cur_policy = Policy::STAND;

	//
	bool boss_attack2 = false;
	//

	int remain_update_cnt_to_change_policy = 0;


	void ForceGravity(const Dungeon* dungeon);
	void AutoAction(const Dungeon* dungeon, const Player* player, AnimationManager* animation_manager, MissileManager* missile_manager, SoundManager* sound_manager);
	void FollowPolicy(const Dungeon* dungeon, const Player* player, AnimationManager* animation_manager, MissileManager* missile_manager, SoundManager* sound_manager);
	void ChooseNewPolicy();

public:
	Monster(const int monster_id, const int width, const int height, const POINT pos,
		const double x_move_px, const double jump_start_power,
		const int hp, const int atk, const int def, const BOOL is_floating, const BOOL melee_attack, const BOOL missile_attack,
		const std::string& stand_animation_name, const std::string& attack_animation_name, const std::string& move_animation_name,
		const TCHAR* start_image_path,
		const POINT policy_stand, const POINT policy_move_to_player, const POINT policy_move_from_player, const POINT policy_attack,
		AnimationManager* animation_manager)
		: Character(monster_id, width, height, pos, State::DOWN, TRUE, x_move_px, jump_start_power,
			stand_animation_name, start_image_path, hp, atk, def, animation_manager),
		is_floating {is_floating}, melee_attack {melee_attack}, missile_attack {missile_attack},
		stand_animation_name{ stand_animation_name }, attack_animation_name{ attack_animation_name }, move_animation_name{ move_animation_name },
		policy_stand{ policy_stand }, policy_move_to_player{ policy_move_to_player }, policy_move_from_player{ policy_move_from_player },
		policy_attack{ policy_attack } 
	{
		animation.LoadAnimation(animation_manager, stand_animation_name);
		animation.Play();
		strcpy_s(atk_sound_name, "sound\\Thunder7.ogg");
		atk_sound_volume = 0.4;
	}

	void Update(const Dungeon* dungeon, const Player* player, AnimationManager* animation_manager, MissileManager* missile_manager, SoundManager* sound_manager);
	void Render(HDC scene_dc, const RECT& bit_rect);
	inline bool IsAppeared() const { return is_appeared; }

	friend class MonsterManager;
	friend class MonsterAI;
};

class MonsterManager : private Uncopyable {
private:
	std::string monster_name;
	int width;
	int height;
	int x_move_px;
	int jump_start_power;
	double x_move_px_double;
	double jump_start_power_double;
	int hp;
	int atk;
	int def;
	BOOL is_floating;
	BOOL melee_attack;
	BOOL missile_attack;
	POINT policy_stand;				// x는 행동을 선택할 확률, y는 행동을 몇 업데이트 카운트 동안 지속할 건지
	POINT policy_move_to_player;
	POINT policy_move_from_player;
	POINT policy_attack;

	int remain_monster_cnt = 0;

	int animation_ids[MONSTER_MAX_ANIMATION_NUM];
	std::string stand_animation_name;
	std::string attack_animation_name;
	std::string move_animation_name;
	TCHAR stand_animation_name_tstr[FILE_NAME_LEN] = L"\0";
	TCHAR attack_animation_name_tstr[FILE_NAME_LEN] = L"\0";
	TCHAR move_animation_name_tstr[FILE_NAME_LEN] = L"\0";

	std::shared_ptr<DB::DataBase> BuildDB();

	void Insert(const Dungeon* dungeon, const int monster_id, int num, AnimationManager* animation_manager);
	void InsertBoss(const Dungeon* dungeon, const int monster_id, AnimationManager* animation_manager);
	void LoadNeededAnimations();
	void BufferEmpty();
	void Clear();

	bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
public:
	std::vector<Monster*> monsters;

	MonsterManager(const Dungeon* dungeon, AnimationManager* animation_manager);
	~MonsterManager();

	void Init(const Dungeon* dungeon, AnimationManager* animation_manager);
	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void Update(const Dungeon* dungeon, const Player* player, AnimationManager* animation_manager, MissileManager* missile_manager, SoundManager* sound_manager);
	void Appear(int num);
	inline bool AreMonsterAllDied() const { return (remain_monster_cnt == 0) ? true : false; }
};
#endif
