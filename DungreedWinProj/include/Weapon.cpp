#include "Weapon.h"

Weapon::Weapon(const Camera* camera, const Player* player, const Crosshair* crosshair, AnimationManager* animation_manager)
	: width {camera->x_half_range / 3}, height {camera->y_half_range / 2}
{
	image = start_image = new Image(L"animation/RustyGreatSwordAttack1.png");
	animation.LoadAnimation(animation_manager, "RustyGreatSwordAttack");
	Update(player, crosshair, animation_manager);
}

Weapon::~Weapon()
{
	delete start_image;
}

void Weapon::Init(const Camera* camera, const Player* player, const Crosshair* crosshair, AnimationManager* animation_manager)
{
	width = camera->x_half_range / 3;
	height = camera->y_half_range / 2;
	Update(player, crosshair, animation_manager);
}

void Weapon::Update(const Player* player, const Crosshair* crosshair, AnimationManager* animation_manager)
{
	pos = player->pos;
	pos.x -= player->width / 4 * 5;
	POINT player_center = { player->pos.x + (player->width / 2), player->pos.y + (player->height / 2) };

	if (player_center.x <= crosshair->pos.x) {
		pos.x += player->width / 4;
		looking_direction = TRUE;
	}
	else {
		pos.x -= player->width / 4;
		looking_direction = FALSE;
	}
	if (player_center.y <= crosshair->pos.y)
		pos.y += player->height / 8;
	else
		pos.y -= player->height / 8;

	angle = Degree(player_center, crosshair->pos);

	UpdateAnimation(animation_manager);

}

void Weapon::UpdateAnimation(AnimationManager* animation_manager)
{
	if (animation.IsPlaying()) {
		if (animation.IsEnd())
			animation.Stop();
		else
			animation.Update();

		image = animation.GetImage(animation_manager);
	}
}

void Weapon::Render(HDC scene_dc, const RECT& bit_rect)
{
	int image_width = image->GetWidth();
	int image_height = image->GetHeight();

	HBITMAP hbm_rotate = RotateImage(scene_dc, image, angle);
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

void Weapon::StartAttack()
{
	animation.Replay();
}

bool Weapon::IsAttackFinished()
{
	if (animation.IsEnd())
		return true;
	else
		return false;
}