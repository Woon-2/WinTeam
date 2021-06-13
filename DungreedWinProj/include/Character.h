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

	Image image;
	std::string old_animation_name;
	std::string cur_animation_name;

	int width;
	int height;
	POINT pos;
	State state = State::DOWN;
	double jump_power = 0;
	BOOL looking_direction;	// TRUE면 오른쪽 보는 상태, FALSE면 왼쪽 보고 있는 상태

	bool is_attacking = false;

	int x_move_px;
	double jump_start_power;
	
	int hp;
	int atk;
	int def;

	const Character* character_who_hit_this = nullptr;

	void Stand();
	void RunLeft();
	void RunRight();
	void DownJump();
	void Jump();

	void MovePos(Direction direction, const int px);

	bool CanGoToPos(const HDC terrain_dc, const POINT pos);
	bool CanGoLeft(const HDC terrain_dc);
	bool CanGoRight(const HDC terrain_dc);
	bool CanDownJump(const HDC terrain_dc);
	bool CanJump(const State state);

	bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
public:
	Character() = default;
	Character(const int id, const int width, const int height, const POINT pos, const State state, const BOOL looking_direction,
		const int x_move_px, const double jump_start_power, const std::string& start_animation_name, const TCHAR* start_image_path,
		const int hp, const int atk, const int def)
		: width{ width }, height{ height }, pos{ pos }, state{ state }, looking_direction{ looking_direction },
		x_move_px{ x_move_px }, jump_start_power{ jump_start_power },
		hp {hp}, atk {atk}, def{def}
	{
		old_animation_name = cur_animation_name = start_animation_name;
		image = Image(start_image_path);
	}

	virtual void Update(const Dungeon* dungeon) {}

	void ForceGravity(const Dungeon* dungeon);

	void Render(HDC scene_dc, const RECT& bit_rect) const;
	void Look(const POINT& target);

	void UpdateAnimation(AnimationManager* animation_manager);

	bool IsOut_Left(const Dungeon* dungeon) const;
	bool IsOut_Right(const Dungeon* dungeon) const;

	void NoOut(const Dungeon* dungeon);

	inline bool IsAttacking() const { return is_attacking; }
	inline bool IsDied() const { return (hp <= 0) ? true : false; }

	friend class HitScanner;
};
#endif
