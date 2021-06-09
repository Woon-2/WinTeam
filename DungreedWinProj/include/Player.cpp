#include "Player.h"

Player::Player(const Dungeon* dungeon)
{
	width = dungeon->camera_x_half_range / PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE;
	height = dungeon->camera_x_half_range / PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE;
}

void Player::PlaceWithDungeonLeft(const Dungeon* dungeon)
{
	pos = dungeon->left_start_pos;
}

void Player::PlaceWithDungeonRight(const Dungeon* dungeon)
{
	pos = dungeon->right_start_pos;
}

void Player::Init(const Dungeon* dungeon)
{
	state = State::DOWN;
	jump_power = 0;
	width = dungeon->camera_x_half_range / PLAYER_WIDTH_PER_CAMERA_X_HALF_RANGE;
	height = dungeon->camera_x_half_range / PLAYER_HEIGHT_PER_CAMERA_Y_HALF_RANGE;
}

void Player::Update(const Dungeon* dungeon, const Crosshair* crosshair)
{
	Look(crosshair->pos);
	KeyProc(dungeon);
	ForceGravity(dungeon);
}

void Player::KeyProc(const Dungeon* dungeon)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	if (GetAsyncKeyState('A')) {
		if (!MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x - 10, pos.y + height / 2 })) {
			MovePos(dungeon, Direction::LEFT, dungeon->camera_x_half_range / 40);
		}
	}
	if (GetAsyncKeyState('D')) {
		if (!MapPixelCollision(dc_set.buf_dc, RGB(255, 0, 0), POINT{ pos.x + width + 10, pos.y + height / 2 })) {
			MovePos(dungeon, Direction::RIGHT, dungeon->camera_x_half_range / 40);
		}
	}

	if ((GetAsyncKeyState('S')) && (GetAsyncKeyState(VK_SPACE))) {
		if (MapPixelCollision(dc_set.buf_dc, RGB(0, 255, 0), POINT{ pos.x,  pos.y + height })) {
			state = State::DOWNJUMP;
		}
	}
	else if (GetAsyncKeyState(VK_SPACE)) {
		if (state == State::LANDING) {
			state = State::UP;
			// 분모가 커지면 점프력이 약해진다.
			jump_power = dungeon->camera_y_half_range / 24.0f;
		}
	}
}