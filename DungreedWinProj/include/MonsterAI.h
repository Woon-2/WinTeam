#pragma once
#ifndef _monsterai
#define _monsterai
#include "Monster.h"
#include "Player.h"
#include "Uncopyable.h"

class Monster;

class MonsterAI : private Uncopyable {
private:
	Monster* monster;

public:
	MonsterAI(Monster* monster) : monster{ monster } {}

	void Stand();
	void MoveToPlayer(const Player* player);
	void MoveFromPlayer(const Player* player);
	void Attack();
};

#endif