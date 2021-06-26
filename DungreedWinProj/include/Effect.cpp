#include "Effect.h"

Effect::Effect(AnimationManager* animation_manager, POINT given_pos, int given_width, int given_height, const std::string& start_animation_name, const TCHAR* start_image_path)
{
	pos = given_pos;
	width = given_width;
	height = given_height;

	//old_animation_name = cur_animation_name = start_animation_name;
	animation_name = start_animation_name;
	image = start_image = new Image(start_image_path);
	//animation_manager->Insert(start_animation_name);
	effect_animaiton.LoadAnimation(animation_manager, start_animation_name);
}

Effect::~Effect()
{
	delete start_image;
}

void Effect::Render(HDC scene_dc, const RECT& bit_rect) const
{
	image->Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image->GetWidth(), image->GetHeight());
}

void Effect::UpdateAnimation(AnimationManager* animation_manager)
{
	effect_animaiton.Play();
	effect_animaiton.Update();
	image = effect_animaiton.GetImage(animation_manager);
}


EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Insert(AnimationManager* animation_manager, POINT given_pos, int given_width, int given_height, const std::string& animation_name, const TCHAR* start_image_path)
{
	Effect* effect = new Effect(animation_manager, given_pos, given_width, given_height, animation_name, start_image_path);
	effects.push_back(effect);
}

void EffectManager::Render(HDC scene_dc, const RECT& bit_rect) const
{
	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->Render(scene_dc, bit_rect);
	}
}

void EffectManager::Update(AnimationManager* animation_manager)
{
	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->UpdateAnimation(animation_manager);

		if (effects[i]->effect_animaiton.IsEnd()) {
			delete effects[i];
			effects.erase(effects.begin() + i);
		}
	}
}