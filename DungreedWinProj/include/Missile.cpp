#include "Missile.h"

void Missile::Update(AnimationManager* animation_manager)
{
	POINT desti_pos;
	if (looking_direction) {
		desti_pos = { static_cast<LONG>(pos.x + speed * cos(radian)), static_cast<LONG>(pos.y - speed * sin(radian)) };
	}
	else {
		desti_pos = { static_cast<LONG>(pos.x - speed * cos(radian)), static_cast<LONG>(pos.y - speed * sin(radian)) };
	}

	pos = desti_pos;

	atk_rect = { pos.x, pos.y, pos.x + width, pos.y + height };

	UpdateAnimation(animation_manager);
}

void Missile::Render(HDC scene_dc, const RECT& bit_rect) const
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();

	HBITMAP hbm_rotate = RotateImage(scene_dc, image, pi / 2 - radian);
	Image* rotate_image = new Image;
	rotate_image->Attach(hbm_rotate);
	rotate_image->SetTransparentColor(RGB(0, 0, 0));

	if (looking_direction) {
		rotate_image->Draw(scene_dc, pos.x, pos.y, width, height, 0, 0, image_width, image_height);
	}
	else {
		DrawFlip(scene_dc, bit_rect, rotate_image, pos, width, height);
	}
	DeleteObject(hbm_rotate);
	delete rotate_image;
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

void Missile::AddAttackVictim(const Character* victim)
{
	attack_victims.push_back(victim);
}

bool Missile::HasAlreadyAttacked(const Character* victim) const
{
	for (const auto& iter : attack_victims)
		if (iter == victim)
			return true;
	return false;
}

bool Missile::IsOutOfRange() const
{
	int dx = pos.x - old_pos.x;
	int dy = pos.y - old_pos.y;
	if (sqrt(dx * dx + dy * dy) > range)
		return true;
	else
		return false;
}

void Missile::UpdateAnimation(AnimationManager* animation_manager)
{
	if (is_animation_load_requested) {
		animation.LoadAnimation(animation_manager, animation_name);
		animation.Play();
		is_animation_load_requested = false;
	}
	else if (animation.IsPlaying()) {
		if (animation.IsEnd())
			animation.Stop();
		else
			animation.Update();
	}

	image = animation.GetImage(animation_manager);
}

void MissileManager::Init()
{
	for (auto* missile : missiles)
		delete missile;
	missiles.clear();
}

void MissileManager::Update(const Dungeon* dungeon, AnimationManager* animation_manager)
{
	InstantDCSet dc_set(RECT{ 0, 0, dungeon->dungeon_width, dungeon->dungeon_height });

	dungeon->dungeon_terrain_image->Draw(dc_set.buf_dc, dc_set.bit_rect);

	for (int i = 0; i < missiles.size(); ++i) {
		auto* missile = missiles.at(i);
		missile->Update(animation_manager);
		if (!CanGoToPos(dc_set.buf_dc, POINT{ missile->pos.x + missile->width / 2, missile->pos.y + missile->height / 2 }) || missile->IsOut_Left(dungeon) || missile->IsOut_Right(dungeon)
			|| missile->IsOutOfRange()) {
			delete missile;
			missiles.erase(missiles.begin() + i);
			--i;
		}
	}
}

void MissileManager::Render(HDC scene_dc, const RECT& bit_rect) const
{
	for (auto* missile : missiles)
		missile->Render(scene_dc, bit_rect);
}

void MissileManager::Insert(Missile* given_missile)
{
	missiles.push_back(given_missile);
}

void MissileManager::Delete(Missile* given_missile)
{
	for (int i = 0; i < missiles.size(); ++i)
		if (missiles.at(i) == given_missile) {
			delete given_missile;
			missiles.erase(missiles.begin() + i);
			break;
		}
}