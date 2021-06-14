#pragma once
#ifndef _monsterai
#define _monsterai
#include "Monster.h"
#include "Player.h"
#include "Dungeon.h"
#include "Uncopyable.h"
#include "InstantDCSet.h"

extern bool MapPixelCollision(const HDC terrain_dc, const COLORREF& val, const POINT& pt);
extern bool CanGoToPos(const HDC terrain_dc, const POINT pos);

class Monster;

class MonsterAI : private Uncopyable {
private:
	Monster* monster;

public:
	MonsterAI(Monster* monster) : monster{ monster } {}

	void Stand();
	void MoveToPlayer(const Dungeon* dungeon, const Player* player);
	void MoveFromPlayer(const Dungeon* dungeon, const Player* player);
	void Attack(const Dungeon* dungeon, const Player* player);
};

#endif