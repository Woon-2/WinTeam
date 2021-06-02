#include "Player.h"

Player::Player(const Dungeon* dungeon) : Character(dungeon)
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

}

void Player::Render()
{

}