#pragma once
#ifndef _hitscan
#define _hitscan
#include "Character.h"
class HitScanner
{
public:
	void operator()(const Character* attacker, Character* hit, const RECT& hit_rect);
	int calc_damage(const Character* attacker, const Character* hit) const;
};

#endif