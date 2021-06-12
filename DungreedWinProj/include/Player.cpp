#include "Player.h"

void Player::PlaceWithDungeonLeft(const Dungeon* dungeon)
{
	pos = dungeon->left_start_pos;
}

void Player::PlaceWithDungeonRight(const Dungeon* dungeon)
{
	pos = dungeon->right_start_pos;
}

void Player::Init(const Dungeon* dungeon, AnimationManager* animation_manager)
{
	state = State::DOWN;
	x_move_px = dungeon->camera_x_half_range / 60.0f;
	jump_start_power = dungeon->camera_y_half_range / 32.0f;
	jump_power = 0;
	dash_power = 0;
	dash_radian = 0;
	width = dungeon->camera_x_half_range / PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE;
	height = dungeon->camera_x_half_range / PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE;
	cur_animation_name = "player_stand";
}

void Player::Update(const Dungeon* dungeon, const Crosshair* crosshair, AnimationManager* animation_manager)
{	
	if (dash_power <= 0) {
		KeyProc(dungeon);
		ForceGravity(dungeon);
		ForceGravity(dungeon);
		Look(crosshair->pos);
	}
	DashProc(Degree(pos, crosshair->pos), dungeon, dungeon->camera_x_half_range / 16);
	MatchStateAndAnimation(animation_manager);
	UpdateAnimation(animation_manager);
}

void Player::KeyProc(const Dungeon* dungeon)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	if (state == State::MOVING && !GetAsyncKeyState('A') && !GetAsyncKeyState('D') && !GetAsyncKeyState('S') && !GetAsyncKeyState(VK_SPACE)) {
		Stand();
		cur_animation_name = "player_stand";
		return;
	}

	if (GetAsyncKeyState('A'))
		if (CanGoLeft(dc_set.buf_dc))
			RunLeft();

	if (GetAsyncKeyState('D'))
		if (CanGoRight(dc_set.buf_dc))
			RunRight();

	if ((GetAsyncKeyState('S')) && (GetAsyncKeyState(VK_SPACE))) {
		if (CanDownJump(dc_set.buf_dc))
			DownJump();
	}
	else if (GetAsyncKeyState(VK_SPACE))
		if (CanJump(state))
			Jump();

	if (IsOut_Left(dungeon) && !dungeon->CanGoPrev())
		MovePos(Direction::RIGHT, x_move_px);
	else if (IsOut_Right(dungeon) && !dungeon->CanGoNext())
		MovePos(Direction::LEFT, x_move_px);
}

void Player::DashProc(float radian, const Dungeon* dungeon, const int px)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	if (dash_power == 0 && GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		dash_power = dungeon->camera_y_half_range / 24.0f;
		dash_radian = radian;
	}

	if (dash_power > 0) {
		POINT old_pos = pos;

		if (looking_direction) {
			POINT desti_pos = { pos.x + px * cos(dash_radian), pos.y - px * sin(dash_radian)};
			POINT desti_character_foot_pos = { desti_pos.x + width / 2, desti_pos.y + height / 5 * 4 };
			if (CanGoToPos(dc_set.buf_dc, desti_character_foot_pos))
				pos = desti_pos;
		}
		else {
			POINT desti_pos = { pos.x - px * cos(dash_radian), pos.y - px * sin(dash_radian)};
			POINT desti_character_foot_pos = { desti_pos.x + width / 2, desti_pos.y + height / 5 * 4 };
			if (CanGoToPos(dc_set.buf_dc, desti_character_foot_pos))
				pos = desti_pos;
		}
		dash_power -= 1;

		if (dash_power < 0) {
			dash_power = -50;
			state == State::DOWN;
		}

		if (IsOut_Left(dungeon) && !dungeon->CanGoPrev())
			pos = old_pos;
		else if (IsOut_Right(dungeon) && !dungeon->CanGoNext())
			pos = old_pos;

	}

	if (dash_power < 0) {
		dash_power++;
		if (dash_power >= 0) {
			dash_power = 0;
		}
	}
}

void Player::MatchStateAndAnimation(AnimationManager* animation_manager)
{
	if (state == State::MOVING)
		cur_animation_name = "player_move";
	else if (state == State::STANDING || state == State::DOWN)
		cur_animation_name = "player_stand";
}