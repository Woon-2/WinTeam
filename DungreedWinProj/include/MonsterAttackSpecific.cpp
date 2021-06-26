#include "MonsterAI.h"
#include "math.h"

void MonsterAI::Attack(const Dungeon* dungeon, const Player* player, AnimationManager* animation_manager, MissileManager* missile_manager, SoundManager* sound_manager)
{
	int dx;
	int dy;
	RECT atk_rect;
	RECT player_rect = { player->pos.x, player->pos.y, player->pos.x + player->width, player->pos.y + player->height };
	RECT tmp;

	switch (monster->id) {
	case 4000001:
		dx = monster->pos.x - player->pos.x;
		dy = monster->pos.y - player->pos.y;
		if (sqrt(dx * dx + dy * dy) > 500) {
			if (monster->animation_name == monster->attack_animation_name) {
				if (!monster->move_animation_name.empty())
					monster->animation_name = monster->move_animation_name;
				else
					monster->animation_name = monster->stand_animation_name;

				monster->is_animation_load_requested = true;
			}
			MoveToPlayer(dungeon, player);
		}
		else if (monster->atk_delay == 0) {
			monster->animation_name = monster->attack_animation_name;
			monster->is_animation_load_requested = true;

			monster->StartAttack(15, 45, RECT{});
			monster->is_attacking = false;
		}
		else if (monster->former_atk_delay == 0) {
			for (double i = -1; i < 1; i += 1.0 / 4.0) {
				missile_manager->Insert(new Missile(monster, monster->pos, monster->width, monster->height / 6 * 5,
					i * pi, 2, 250, TRUE, 1, 25, L"animation/BansheeBullet1.png", "BansheeBullet", animation_manager,
					"sound\\Water1.ogg", 0.4));
			}
		}
		break;
	case 4000002:
		if (!monster->is_attacking) {
			if (monster->looking_direction)
				atk_rect = { monster->pos.x + monster->width / 3, monster->pos.y, monster->pos.x + monster->width, monster->pos.y + monster->height };
			else
				atk_rect = { monster->pos.x, monster->pos.y, monster->pos.x + monster->width / 3 * 2, monster->pos.y + monster->height };

			if (IntersectRect(&tmp, &atk_rect, &player_rect)) {
				monster->animation_name = monster->attack_animation_name;
				monster->is_animation_load_requested = true;

				monster->StartAttack(40, 60, atk_rect);
			}
			else {
				if (monster->animation_name == monster->attack_animation_name) {
					if (!monster->move_animation_name.empty())
						monster->animation_name = monster->move_animation_name;
					else
						monster->animation_name = monster->stand_animation_name;

					monster->is_animation_load_requested = true;
				}
				MoveToPlayer(dungeon, player);
			}
		}
		else if (monster->animation.IsEnd()) {
			monster->FinishAttack();
		}
		break;
	case 4000013:
		if (monster->atk_delay == 0) {
			monster->animation_name = monster->attack_animation_name;
			monster->is_animation_load_requested = true;

			if (!monster->boss_attack2) {
				monster->StartAttack(40, 60, RECT{});
			}
			else {
				monster->StartAttack(10, 11, RECT{});
				monster->is_animation_load_requested = false;
			}
			monster->is_attacking = false;
		}
		else if (monster->former_atk_delay == 0) {

			if (!monster->boss_attack2) {
				for (double i = -1; i < 1; i += 1.0 / 4.0) {
					missile_manager->Insert(new Missile(monster,
						POINT{monster->pos.x + monster->width / 2, monster->pos.y + monster->height / 2}
						, monster->width / 5, monster->height / 5,
						i * pi, 4, 500, TRUE, 1, 15, L"animation/SkellBossBullet1.png", "SkellBossBullet", animation_manager,
						"sound\\Explosion1.ogg", 0.4));
				}
			}
			else {
				if (player->pos.x + player->width / 2 > monster->pos.x + monster->width / 2) {
					missile_manager->Insert(new Missile(monster,
						POINT{ monster->pos.x + monster->width / 2, monster->pos.y + monster->height / 2 }
						, monster->width / 5, monster->height / 5,
						Degree(POINT{ player->pos.x + player->width / 2, player->pos.y + player->height / 2 },
							POINT{ monster->pos.x + monster->width / 2, monster->pos.y + monster->height / 2 })
						, 4, 500, TRUE, 1, 15, L"animation/SkellBossBullet1.png",
						"SkellBossBullet", animation_manager, "sound\\Explosion1.ogg", 0.4));

				}
				else {
					missile_manager->Insert(new Missile(monster,
						POINT{ monster->pos.x + monster->width / 2, monster->pos.y + monster->height / 2 }
						, monster->width / 5, monster->height / 5,
						Degree(POINT{ player->pos.x + player->width / 2, player->pos.y + player->height / 2 },
							POINT{ monster->pos.x + monster->width / 2, monster->pos.y + monster->height / 2 })
						, 4, 500, FALSE, 1, 15, L"animation/SkellBossBullet1.png",
						"SkellBossBullet", animation_manager, "sound\\Explosion1.ogg", 0.8));
				}
			}

		}
		break;
	}
}