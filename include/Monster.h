#pragma once
#ifndef _monster
#define _monster
#include "Character.h"
class Monster : public Character
{
private:
public:
	Monster(const Dungeon* dungeon);

	void Update(const Dungeon* dungeon);
	void Render();

};
#endif
