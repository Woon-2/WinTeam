#include "Weapon.h"

Weapon::Weapon(const Camera* camera, const Player* player, const Crosshair* crosshair)
{
	image = new Image(L"sword\\RustyGreatSword00-resources.assets-1011.png");
	width = camera->x_half_range / 3;
	height = camera->y_half_range / 2;
	Update(player, crosshair);
}

void Weapon::Init(const Camera* camera, const Player* player, const Crosshair* crosshair)
{
	width = camera->x_half_range / 3;
	height = camera->y_half_range / 2;
	Update(player, crosshair);
}

float Degree(const POINT& point1, const POINT& point2)
{
	float degree = atan2(point2.y - point1.y, point2.x - point1.x);
	//degree = degree * 180 / (std::atan(1) * 4);	// atan(1) * 4 = PI

	return degree;
}

void Weapon::Update(const Player* player, const Crosshair* crosshair)
{
	pos = player->pos;
	pos.x -= player->width;
	POINT player_center = { player->pos.x + (player->width / 2), player->pos.y + (player->height / 2) };
	angle = Degree(player_center, crosshair->pos);
}

void Weapon::Render(HDC scene_dc, const RECT& bit_rect)
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