#include "MonsterAI.h"

void MonsterAI::Stand()
{
	monster->Stand();
}

void MonsterAI::MoveToPlayer(const Dungeon* dungeon, const Player* player)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });
	POINT old_pos = monster->pos;

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	monster->Look(player->pos);

	if (!monster->is_floating) {						// 떠다니지 않는 몬스터의 이동
		if (monster->looking_direction) {
			if (monster->CanGoRight(dc_set.buf_dc))
				monster->RunRight();
			else if (monster->CanJump(monster->state))
				monster->Jump();
		}
		else {
			if (monster->CanGoLeft(dc_set.buf_dc))
				monster->RunLeft();
			else if (monster->CanJump(monster->state))
				monster->Jump();
		}
	}
	else {												// 떠다니는 몬스터의 이동
		double radian = Degree(player->pos, monster->pos);
		POINT desti_pos;
		POINT desti_pos_center;
		double dx;
		double dy;

		if (monster->looking_direction) {
			dx = monster->x_move_px * cos(radian);
			dy = -monster->x_move_px * sin(radian);
			desti_pos = { static_cast<int>(monster->pos.x + dx), static_cast<int>(monster->pos.y + dy) };
		}
		else {
			dx = -monster->x_move_px * cos(radian);
			dy = -monster->x_move_px * sin(radian);
			desti_pos = { static_cast<int>(monster->pos.x + dx), static_cast<int>(monster->pos.y + dy) };
		}

		desti_pos_center = { desti_pos.x + monster->width / 2, desti_pos.y + monster->height / 2 };

		if (CanGoToPos(dc_set.buf_dc, desti_pos_center))
			monster->pos = desti_pos;
	}

	if (monster->IsOut_Left(dungeon))
		monster->pos = old_pos;
	else if (monster->IsOut_Right(dungeon))
		monster->pos = old_pos;
}

void MonsterAI::MoveFromPlayer(const Dungeon* dungeon, const Player* player)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });
	POINT old_pos = monster->pos;

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	monster->Look(player->pos);
	monster->looking_direction = (monster->looking_direction == TRUE) ? FALSE : TRUE;

	if (!monster->is_floating) {						// 떠다니지 않는 몬스터의 이동
		if (monster->looking_direction) {
			if (monster->CanGoRight(dc_set.buf_dc) && !CanGoToPos(dc_set.buf_dc,
				POINT {monster->pos.x + monster->width / 2 + static_cast<LONG>(monster->x_move_px), monster->pos.y + monster->height + dungeon->dungeon_height / 50}))
				monster->RunRight();
		}
		else {
			if (monster->CanGoLeft(dc_set.buf_dc) && !CanGoToPos(dc_set.buf_dc,
				POINT{ monster->pos.x + monster->width / 2 - static_cast<LONG>(monster->x_move_px), monster->pos.y + monster->height + dungeon->dungeon_height / 50 }))
				monster->RunLeft();
		}
	}
	else {												// 떠다니는 몬스터의 이동
		double radian = Degree(monster->pos, player->pos);
		POINT desti_pos;
		POINT desti_pos_center;
		double dx;
		double dy;

		if (monster->looking_direction) {
			dx = monster->x_move_px * cos(radian);
			dy = - monster->x_move_px * sin(radian);
			desti_pos = { static_cast<int>(monster->pos.x + dx), static_cast<int>(monster->pos.y + dy) };
		}
		else {
			dx = -monster->x_move_px * cos(radian);
			dy = -monster->x_move_px * sin(radian);
			desti_pos = { static_cast<int>(monster->pos.x + dx), static_cast<int>(monster->pos.y + dy) };
		}

		desti_pos_center = { desti_pos.x + monster->width / 2, desti_pos.y + monster->height / 2 };

		if (CanGoToPos(dc_set.buf_dc, desti_pos_center))
			monster->pos = desti_pos;
	}

	if (monster->IsOut_Left(dungeon))
		monster->pos = old_pos;
	else if (monster->IsOut_Right(dungeon))
		monster->pos = old_pos;

}
