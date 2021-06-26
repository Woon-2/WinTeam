#pragma once
#ifndef _hitscan
#define _hitscan
#include "Character.h"
#include "Missile.h"
#include "Sound.h"

class HitScanner
{
public:
	void operator()(Character* attacker, Character* victim, SoundManager* sound_manager);
	void operator()(Missile* attacker, Character* victim, MissileManager* missile_manager, SoundManager* sound_manager);
	int calc_damage(const Character* attacker, const Character* victim);
	int calc_damage(const Missile* attacker, const Character* victim);
};

#endif