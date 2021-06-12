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
	width = dungeon->camera_x_half_range / PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE;
	height = dungeon->camera_x_half_range / PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE;
	cur_animation_name = "player_stand";
}

void Player::Update(const Dungeon* dungeon, const Crosshair* crosshair, AnimationManager* animation_manager)
{	
	KeyProc(dungeon);
	ForceGravity(dungeon);
	ForceGravity(dungeon);
	Look(crosshair->pos);
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

void Player::MatchStateAndAnimation(AnimationManager* animation_manager)
{
	if (state == State::MOVING)
		cur_animation_name = "player_move";
	else if (state == State::STANDING || state == State::DOWN)
		cur_animation_name = "player_stand";
}