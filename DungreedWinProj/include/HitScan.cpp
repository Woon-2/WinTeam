#include "HitScan.h"

void HitScanner::operator()(Character* attacker, Character* victim, SoundManager* sound_manager)
{
	if (attacker->IsAttacking() && attacker->former_atk_delay <= 0 && !attacker->HasAlreadyAttacked(victim)) {
		RECT my_rect = { victim->pos.x, victim->pos.y, victim->pos.x + victim->width, victim->pos.y + victim->height };
		RECT tmp;
		if (IntersectRect(&tmp, &my_rect, &attacker->atk_rect)) {
			sound_manager->Play(attacker->atk_sound_name, attacker->atk_sound_volume);
			attacker->AddAttackVictim(victim);
			victim->hp -= calc_damage(attacker, victim);
			victim->red_flash_cnt = 8;
		}
	}
}

void HitScanner::operator()(Missile* attacker, Character* victim, MissileManager* missile_manager, SoundManager* sound_manager)
{
	if (!attacker->HasAlreadyAttacked(victim)) {
		RECT my_rect = { victim->pos.x, victim->pos.y, victim->pos.x + victim->width, victim->pos.y + victim->height };
		RECT tmp;
		if (IntersectRect(&tmp, &my_rect, &attacker->atk_rect)) {
			sound_manager->Play(attacker->atk_sound_name, attacker->atk_sound_volume);
			attacker->AddAttackVictim(victim);
			victim->hp -= calc_damage(attacker, victim);
			victim->red_flash_cnt = 8;
			if (attacker->pierce == attacker->attack_victims.size())
				missile_manager->Delete(attacker);
		}
	}
}

int HitScanner::calc_damage(const Character* attacker, const Character* victim)
{
	return attacker->atk * victim->def / (victim->def + 100);
}

int HitScanner::calc_damage(const Missile* attacker, const Character* victim)
{
	return attacker->atk * victim->def / (victim->def + 100);
}