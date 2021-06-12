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
	if (dash_power <= 0) {
		Look(crosshair->pos);
		KeyProc(dungeon);
		ForceGravity(dungeon);
	}
	Dash(Degree(pos, crosshair->pos), dungeon, 30);
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

void Player::Dash(float radian, const Dungeon* dungeon, const int px)
{
	if (dash_power == 0 && GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		dash_power = dungeon->camera_y_half_range / 24.0f;
		dash_radian = radian;
	}

	if (dash_power > 0) {
		if (looking_direction) {
			pos.x += px * cos(dash_radian);
			pos.y -= px * sin(dash_radian);
		}
		else {
			pos.x -= px * cos(dash_radian);
			pos.y -= px * sin(dash_radian);
		}
		dash_power -= 1;

		if (dash_power < 0) {
			dash_power = -50;
			state == State::DOWN;
		}
	}

	if (dash_power < 0) {
		dash_power++;
		if (dash_power >= 0) {
			dash_power = 0;
		}
	}
}

bool Player::IsOut_Left(const Dungeon* dungeon) const
{
	if (pos.x < 0)
		return true;
	else
		return false;
}

bool Player::IsOut_Right(const Dungeon* dungeon) const
{
	if (pos.x + width > dungeon->dungeon_width)
		return true;
	else
		return false;
}