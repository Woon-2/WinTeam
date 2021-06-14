#include "Crosshair.h"

Crosshair::Crosshair(const Camera* camera) : half_size { camera->x_half_range / 20 }
{
	Update(camera);
	ShowCursor(FALSE);
	image = new Image(L"ShootingCursor1-resources.assets-2645.png");
}

Crosshair::~Crosshair()
{
	delete image;
}

void Crosshair::Init(const Camera* camera)
{
	half_size = camera->x_half_range / 20.0f;
	Update(camera);
}

void Crosshair::Update(const Camera* camera)
{
	GetCursorPos(&pos);
	ScreenToClient(h_wnd, &pos);

	pos.x *= camera->x_half_range * 2 / static_cast<double>(client.right);
	pos.y *= camera->y_half_range * 2 / static_cast<double>(client.bottom);
	pos.x += camera->pos.x - camera->x_half_range;
	pos.y += camera->pos.y - camera->y_half_range;
}

void Crosshair::Render(HDC scene_dc, const RECT& bit_rect)
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();
	image->Draw(scene_dc, pos.x - half_size, pos.y - half_size, half_size * 2, half_size * 2, 0, 0, image_width, image_height);
}