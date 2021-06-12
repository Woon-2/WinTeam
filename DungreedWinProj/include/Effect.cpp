#include "Effect.h"

void Effect::Init(AnimationManager* animation_manager, POINT given_pos, int given_width, int given_height, const std::string& start_animation_name, const TCHAR* start_image_path)
{
	pos = given_pos;
	width = given_width;
	height = given_height;

	old_animation_name = cur_animation_name = start_animation_name;
	image = Image(start_image_path);
}

void Effect::Render(HDC scene_dc, const RECT& bit_rect) const
{
	image.Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image.GetWidth(), image.GetHeight());
}

void Effect::UpdateAnimation(AnimationManager* animation_manager)
{
	if (old_animation_name != cur_animation_name) {
		animation_manager->Stop(old_animation_name);
		animation_manager->Play(cur_animation_name);
		animation_manager->Update();

		old_animation_name = cur_animation_name;
	}

	image = animation_manager->GetImage(cur_animation_name);
}