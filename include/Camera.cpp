#include "Camera.h"

Camera::Camera(const Dungeon* dungeon, const Player* player)
	: x_half_range{ dungeon->camera_x_half_range }, y_half_range{ dungeon->camera_y_half_range }
{
	Update(dungeon, player);
}

void Camera::Init(const Dungeon* dungeon, const Player* player)
{
	x_half_range = dungeon->camera_x_half_range;
	y_half_range = dungeon->camera_y_half_range;
	Update(dungeon, player);
}

void Camera::Update(const Dungeon* dungeon, const Player* player)
{
	if (player->pos.x < x_half_range) {
		pos.x = x_half_range;
	}
	else if (player->pos.x > dungeon->dungeon_width - x_half_range) {
		pos.x = dungeon->dungeon_width - x_half_range;
	}
	else {
		pos.x = player->pos.x;
	}

	if (player->pos.y < y_half_range) {
		pos.y = y_half_range;
	}
	else if (player->pos.y > dungeon->dungeon_height - y_half_range) {
		pos.y = dungeon->dungeon_height - y_half_range;
	}
	else {
		pos.y = player->pos.y;
	}
}