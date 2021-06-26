#pragma once
#ifndef _character
#define _character
#include <windows.h>
#include <string>
#include "FileUtility.h"
#include "Dungeon.h"
#include "InstantDCSet.h"
#include "Animation.h"

extern HDC buf_dc;
extern HWND h_wnd;
extern RECT client;

extern bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
extern bool CanGoToPos(const HDC terrain_dc, const POINT pos);

class Attack;

class Character
{
protected:
	enum class State {
		DOWN, UP, STANDING, MOVING, DOWNJUMP
	};
	enum class Direction {
		LEFT, UP, RIGHT, DOWN
	};

	int id;

	const Image* image;
	const Image* start_image;

	Animation animation;

	bool is_animation_load_requested = false;
	std::string animation_name;

	int width;
	int height;
	POINT pos;
	State state = State::DOWN;
	double jump_power = 0;
	BOOL looking_direction;	// TRUE면 오른쪽 보는 상태, FALSE면 왼쪽 보고 있는 상태

	double x_move_px;
	double jump_start_power;
	
	int hp;
	int max_hp;
	int atk;
	int def;

	bool is_attacking = false;
	std::vector<const Character*> attack_victims;
	RECT atk_rect;
	int former_atk_delay = 0;
	int atk_delay = 0;
	int red_flash_cnt = 0;
	char atk_sound_name[FILE_NAME_LEN];
	float atk_sound_volume;

	void Stand();
	void RunLeft();
	void RunRight();
	void DownJump();
	void Jump();

	void MovePos(Direction direction, const int px);

	bool CanGoLeft(const HDC terrain_dc);
	bool CanGoRight(const HDC terrain_dc);
	bool CanDownJump(const HDC terrain_dc);
	bool CanJump(const State state);

	void StartAttack(const int given_former_atk_delay, const int given_atk_delay, const RECT& given_atk_rect);
	void FinishAttack();
	virtual void UpdateAtkRect() {}
	void AddAttackVictim(const Character* victim);
	bool HasAlreadyAttacked(const Character* victim) const;
	inline bool IsAttacking() const { return is_attacking; }

public:
	Character() = default;
	Character(const int id, const int width, const int height, const POINT pos, const State state, const BOOL looking_direction,
		const double x_move_px, const double jump_start_power, const std::string& start_animation_name, const TCHAR* start_image_path,
		const int hp, const int atk, const int def, AnimationManager* animation_manager)
		: id{ id }, width {width}, height{ height }, pos{ pos }, state{ state }, looking_direction{ looking_direction },
		x_move_px{ x_move_px }, jump_start_power{ jump_start_power },
		hp {hp}, atk {atk}, def{def}, max_hp {hp}
	{
		animation.LoadAnimation(animation_manager, start_animation_name);
		animation_name = start_animation_name;
		image = start_image = new Image(start_image_path);
	}
	~Character() { delete start_image; }

	virtual void Update(const Dungeon* dungeon) {}

	void ForceGravity(const Dungeon* dungeon);

	void Render(HDC scene_dc, const RECT& bit_rect);
	void RenderMonsterHP(HDC scene_dc, const RECT& bit_rect) const;
	void RenderPlayerHP(HDC scene_dc, const RECT& bit_rect, const RECT& camera) const;

	void Look(const POINT& target);
	void Look(const Character& target);

	void UpdateAnimation(AnimationManager* animation_manager);

	bool IsOut_Left(const Dungeon* dungeon) const;
	bool IsOut_Right(const Dungeon* dungeon) const;

	void NoOut(const Dungeon* dungeon);

	inline bool IsDied() const { return (hp <= 0) ? true : false; }

	friend class HitScanner;
	friend class MonsterAI;
};
#endif
