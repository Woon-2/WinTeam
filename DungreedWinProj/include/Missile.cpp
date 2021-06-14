#include "Missile.h"

void Missile::Update(float radian, const Dungeon* dungeon)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image.Draw(dc_set.buf_dc, dc_set.bit_rect);

	POINT old_pos = pos;

	if (looking_direction) {
		POINT desti_pos = { pos.x + speed * cos(radian), pos.y - speed * sin(radian) };
		POINT desti_center_pos = { desti_pos.x + width / 2, desti_pos.y + height / 2 };
		if (CanGoToPos(dc_set.buf_dc, desti_center_pos))
			pos = desti_pos;
	}
	else {
		POINT desti_pos = { pos.x - speed * cos(radian), pos.y - speed * sin(radian) };
		POINT desti_center_pos = { desti_pos.x + width / 2, desti_pos.y + height / 2 };
		if (CanGoToPos(dc_set.buf_dc, desti_center_pos))
			pos = desti_pos;
	}

	if (IsOut_Left(dungeon))
		pos = old_pos;
	else if (IsOut_Right(dungeon))
		pos = old_pos;
}

void Missile::Render(HDC scene_dc) const
{
	image.Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image.GetWidth(), image.GetHeight());
}

bool Missile::IsOut_Left(const Dungeon* dungeon) const
{
	if (pos.x < 0)
		return true;
	else
		return false;
}

bool Missile::IsOut_Right(const Dungeon* dungeon) const
{
	if (pos.x + width > dungeon->dungeon_width)
		return true;
	else
		return false;
}