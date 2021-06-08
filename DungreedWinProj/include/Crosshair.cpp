#include "Crosshair.h"

Crosshair::Crosshair(const RECT camera_rect, const Dungeon* dungeon, const RECT& client)
{
	half_size = dungeon->camera_x_half_range / 20;
	Update(camera_rect, dungeon, client);
	ShowCursor(FALSE);
}

void Crosshair::Update(const RECT camera_rect, const Dungeon* dungeon, const RECT& client)
{
	
	GetCursorPos(&pos);
	ScreenToClient(h_wnd, &pos);
	
	pos.x *= static_cast<double>(dungeon->camera_x_half_range * 2) / static_cast<double>(client.right);
	pos.y *= static_cast<double>(dungeon->camera_y_half_range * 2) / static_cast<double>(client.bottom);
	pos.x += camera_rect.left;
	pos.y += camera_rect.top;
}

void Crosshair::Render(HDC scene_dc, const RECT& bit_rect)
{
	Image image(L"ShootingCursor1-resources.assets-2645.png");
	int image_width = image.GetWidth();
	int image_height = image.GetHeight();
	image.Draw(scene_dc, pos.x - half_size, pos.y - half_size, half_size * 2, half_size * 2, 0, 0, image_width, image_height);

}

BOOL Crosshair::LookingDirection(const POINT* player)
{
	if (pos.x > player->x) {
		return TRUE;
	}
	return FALSE;
}