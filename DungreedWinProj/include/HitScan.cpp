#include "HitScan.h"

void HitScanner::operator()(const Character* attacker, Character* hit, const RECT& hit_rect)
{
	if (attacker->IsAttacking()) {
		RECT my_rect = { hit->pos.x, hit->pos.y, hit->pos.x + hit->width, hit->pos.y + hit->height };
		RECT tmp;
		if (IntersectRect(&tmp, &my_rect, &hit_rect)) {
			hit->character_who_hit_this = attacker;
			hit->hp -= calc_damage(attacker, hit);
			// ÇÇ°İ ÀÌÆåÆ®
		}
	}
	else if (attacker == hit->character_who_hit_this)
		hit->character_who_hit_this = nullptr;
}

int HitScanner::calc_damage(const Character* attacker, const Character* hit) const
{
	return attacker->atk * hit->def / (hit->def + 100);
}