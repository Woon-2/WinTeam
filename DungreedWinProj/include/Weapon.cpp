#include "Weapon.h"

Weapon::Weapon(const Dungeon* dungeon, const POINT* player, const int* player_width, const int* player_height, const POINT* crosshair)
{
	image = new Image(L"sword\\RustyGreatSword00-resources.assets-1011.png");
	width = dungeon->camera_x_half_range / 3;
	height = dungeon->camera_y_half_range / 2;
	Update(player, player_width, player_height, crosshair);
}

float Degree(const POINT* point1, const POINT* point2)
{
	float degree = atan2(point2->y - point1->y, point2->x - point1->x);
	//degree = degree * 180 / (std::atan(1) * 4);	// atan(1) * 4 = PI

	return degree;
}

void Weapon::Update(const POINT* player, const int* player_width, const int* player_height, const POINT* crosshair)	// 각도, 좌표 업데이트
{
	pos = *player;
	pos.x -= *player_width;
	POINT player_center = { player->x + (*player_width / 2), player->y + (*player_height / 2) };
	angle = Degree(&player_center, crosshair);
	
}

void FlipImage(HDC scene_dc, const RECT& bit_rect, Image* image, int x, int y, int width, int height);

void Weapon::Render(HDC scene_dc, const RECT& bit_rect, BOOL looking_direction)
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();
	
	//TCHAR lpOut[200];
	//wsprintf(lpOut, TEXT("%d"), static_cast <int> (angle));
	//TextOut(scene_dc, 300, 100, lpOut, 10);

	if (looking_direction) {
		FlipImage(scene_dc, bit_rect, image, pos.x, pos.y, width, height);
	}
	else {
		image->Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image_width, image_height);
	}
	//RotateImage(scene_dc, bit_rect, image, pos.x, pos.y, width, height, angle, looking_direction);
}